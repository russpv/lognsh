#include "parse_int.h"

#define ERRMSG_PATH_MALLOC "Allocation for path value failed.\n"
#define ERRMSG_NULL_EXPAN "Null expansion variable.\n"

#define DEBUGMSG_REDIRP_ANNOUNCE "Parser: p_do_redir_processing...\n"
#define DBGMSG_REDIRP_GOT "Parser: p_do_redir_processing got: glob_%d exp_%d\n"

#define DEBUGMSG_DOEXP_ANNOUNCE "Parser: p_do_expansion got: %s\n"
#define DEBUGMSG_DOEXP_RES "Parser: p_do_expansion found: %s\n"

/* Looks for env values of key loaded in buf */
static int	_do_redir_ops(t_state *s, const t_redir_data *r, char *buf, char **value)
{
	char	*new_fn;
	if (0 == check_special_expansions(s, buf, value))
	{
		if (*value)
		{
			free(r->filename);
			((t_redir_data *)r)->filename = *value;
		}
	}
	else
	{
		*value = get_sh_env(s, buf);
		if (*value)
		{
			new_fn = ft_strdup(*value);
			if (!new_fn)
				return (err(ERRMSG_PATH_MALLOC), ERR_MEM);
			free(r->filename);
			((t_redir_data *)r)->filename = new_fn;
		}
	}
	return (0);
}

// TODO consider how to process HEREDOCS
/* Replaces $vars with expanded strings
 * Passed to t_ast_node.cmd's t_list*args iterator.
 * If flag set, looks for expansion values in shell envp
 * by passing a buffer loaded with the key and replaces
 * t_arg_data.raw string, omitting any '$'.
 * The string to expand must be more than LEXERKEEP$ char (for '$').
 */
static int	_p_do_red_expansion(t_state *s, void *c)
{
	char				*value;
	const t_redir_data	*content = (t_redir_data *)c;
	int					res;
	char				buf[MAX_ENVVAR_LEN];
	size_t				fn_len;

	res = 0;
	value = NULL;
	fn_len = ft_strnlen(content->filename, MAX_ENVVAR_LEN);
	ft_memset(buf, 0, sizeof(buf));
	debug_print(DEBUGMSG_DOEXP_ANNOUNCE, content->filename);
	if (content->do_expansion)
	{
		if (fn_len <= LEXERKEEP$)
			return (err(ERRMSG_NULL_EXPAN), ERR_ARGS);
		ft_memcpy(buf, content->filename + LEXERKEEP$, fn_len - LEXERKEEP$);
		res = _do_redir_ops(s, content, buf, &value);
		if (0 != res)
			return (res);
		debug_print(DEBUGMSG_DOEXP_RES, value);
	}
	return (0);
}

/* Modifies redirection list in case of expansions.
 * (No need to free orig_fn, command cleanup will get it)
 */
int	p_do_redir_processing(t_state *s, t_ast_node *a)
{
	t_list	*redirs;
	char	*orig_filen;
	int		res;

	if (a->type != AST_NODE_CMD)
		return (ERR_ARGS);
	orig_filen = NULL;
	redirs = p_get_redirs(a);
	res = 0;
	if (redirs)
	{
		debug_print(DEBUGMSG_REDIRP_ANNOUNCE);
		debug_print(DBGMSG_REDIRP_GOT, a->data.cmd.do_redir_globbing, a->data.cmd.do_redir_expansion);
		if (true == a->data.cmd.do_redir_expansion)
			res = lstiter_state(s, redirs, _p_do_red_expansion);
		if (0 != res)
			return (res);
		if (true == a->data.cmd.do_redir_globbing)
			orig_filen = ft_lstiterstr(redirs, p_do_globbing_redirs);
		if (NULL != orig_filen)
		{
			print_ambiguous_redirect(((t_redir_data *)orig_filen)->filename);
			return (ERR_AMBIGUOUS_REDIR);
		}
	}
	return (0);
}
