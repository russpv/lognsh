#include "parse_int.h"

# define ERRMSG_PATH_MALLOC "Allocation for path value failed.\n"
# define ERRMSG_NULL_EXPAN "Null expansion variable.\n"

#define DEBUGMSG_ARGP_PRE_G "Parser: Pre-glob headp: %p, 1st node: %p\n"
#define DEBUGMSG_ARGP_POST_G "Parser: Post-glob headp: %p, 1st node: %p\n"
#define DEBUGMSG_REDIRP_ANNOUNCE "Parser: p_do_redir_processing...\n"
#define DEBUGMSG_CHKSPEC_ANNOUNCE "Parser: _check_special_expansions got: %s\n"
#define DEBUGMSG_DOEXP_ANNOUNCE "Parser: p_do_expansion got: %s\n"
#define DEBUGMSG_DOEXP_RES "Parser: p_do_expansion found: %s\n"

/* Add special $? codes here. 
 * buf will start with char after the '$'.
 * Returns the expanded heap string.
 */
static int	_check_special_expansions(t_arg_data *data, const char *buf,\
		char **value)
{
	const int	*status = get_status(data->global_state);

	if (!buf || !value)
		return (ERR_ARGS);
	if (ft_strcmp(buf, "?") == 0)
	{
		if (status == NULL)
			*value = ft_itoa(0);
		else
			*value = ft_itoa(*status);
		if (*value == NULL)
			return (ERR_MEM);
		debug_print(DEBUGMSG_CHKSPEC_ANNOUNCE, *value);
		return (0);
	}
	return (0);
}

static int	_do_ops(t_state *s, const t_arg_data *c, char *buf, char **value)
{
	char				*new_raw;

	if (0 == _check_special_expansions((t_arg_data*)c, buf, value))
	{
		if (*value)
		{
			free(c->raw);
			((t_arg_data *)c)->raw = *value;
		}
	}
	else
	{
		*value = get_sh_env(s, buf);
		if (*value)
		{
			new_raw = ft_strdup(*value);
			if (!new_raw)
				return (err(ERRMSG_PATH_MALLOC), ERR_MEM);
			free(c->raw);
			((t_arg_data *)c)->raw = new_raw;
		}
	}
	return (0);
}

/* Passed to t_ast_node.cmd's t_list*args iterator.
 * If flag set, looks for expansion values in shell envp and replaces
 * t_arg_data.raw string, omitting '$'.
 */
static int	_p_do_expansion(t_state *s, void *c)
{
	char				*value;
	const t_arg_data	*content = (t_arg_data *)c;
	int					res;
	char				buf[MAX_INPUT];
	size_t				raw_len;

	res = 0;
	value = NULL;
	raw_len = ft_strnlen(content->raw, MAX_INPUT);
	if (raw_len <= 1)
		return (err(ERRMSG_NULL_EXPAN), ERR_ARGS);
	ft_memset(buf, 0, sizeof(buf));
	ft_memcpy(buf, content->raw + 1, raw_len - 1);
	debug_print(DEBUGMSG_DOEXP_ANNOUNCE, content->raw);
	if (content->do_expansion)
	{
		res =_do_ops(s, content, buf, &value);
		if (0 != res)
			return(res);
		debug_print(DEBUGMSG_DOEXP_RES, value);
	}
	return (0);
}

/* Replaces any expansions and inserts any globbing 
 * results into args list, converts to array 
 * and returns that array.
 */
int	p_do_arg_processing(t_state *s, t_ast_node *a, char ***args)
{
	t_list	**argl;
	int		res;

	if (a->type != AST_NODE_CMD)
		return (ERR_ARGS);
	argl = p_get_args(a);
	if (argl)
	{
		res = lstiter_state(s, *argl, _p_do_expansion);
		if (0 != res)
			return (res);
		debug_print(DEBUGMSG_ARGP_PRE_G, argl, *argl);
		ft_lstiter_ins_rwd(argl, p_do_globbing);
		debug_print(DEBUGMSG_ARGP_POST_G, argl, *argl);
		a->data.cmd.argc = ft_lstsize(*argl);
		*args = list_to_array(*argl, a->data.cmd.argc);
		if (NULL == *args)
			return (ERR_MEM);
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
	redirs = p_get_redirs(a);
	res = lstiter_state(s, redirs, _p_do_expansion);
	if (0 != res)
		return (res);
	debug_print(DEBUGMSG_REDIRP_ANNOUNCE);
	orig_filen = ft_lstiterstr(redirs, p_do_globbing_redirs);
	if (NULL != orig_filen)
	{
		print_ambiguous_redirect(((t_redir_data *)orig_filen)->filename);
		return (ERR_AMBIGUOUS_REDIR);
	}
	return (0);
}
