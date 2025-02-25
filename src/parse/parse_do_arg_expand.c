#include "parse_int.h"

#define ERRMSG_PATH_MALLOC "Allocation for path value failed.\n"
#define ERRMSG_NULL_EXPAN "Null expansion variable.\n"

#define DEBUGMSG_ARGP_PRE_G "Parser: Pre-glob headp: %p, 1st node: %p\n"
#define DEBUGMSG_ARGP_POST_G "Parser: Post-glob headp: %p, 1st node: %p\n"

#define DEBUGMSG_CHKSPEC_ANNOUNCE "Parser: _check_special_expansions got: %s\n"

#define DEBUGMSG_DOEXP_ANNOUNCE "Parser: p_do_expansion got: %s\n"
#define DEBUGMSG_DOEXP_RES "Parser: p_do_expansion found: %s\n"

/* Add special $? codes here.
 * buf will start with char after any '$'.
 * Returns 0 if special expansion done or no last exit status.
 * Places the expanded heap string in *value
 */
int	check_special_expansions(t_state *s, const char *buf,
	char **value)
{
	const int	*status = get_status(s);

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
	return (1);
}

/* Looks for env values of key loaded in buf */
static int	_do_arg_ops(t_state *s, const t_arg_data *c, char *buf, char **value)
{
	char	*new_raw;

	if (0 == check_special_expansions(s, buf, value))
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
 * If flag set, looks for expansion values in shell envp
 * by passing a buffer loaded with the key and replaces
 * t_arg_data.raw string, omitting any '$'.
 * The string to expand must be more than LEXERKEEP$ char (for '$').
 * Responsible for BOTH args and redirs.
 */
static int	_p_do_arg_expansion(t_state *s, void *c)
{
	char				*value;
	const t_arg_data	*content = (t_arg_data *)c;
	int					res;
	char				buf[MAX_ENVVAR_LEN];
	size_t				raw_len;

	res = 0;
	value = NULL;
	raw_len = ft_strnlen(content->raw, MAX_ENVVAR_LEN);
	ft_memset(buf, 0, sizeof(buf));
	debug_print(DEBUGMSG_DOEXP_ANNOUNCE, content->raw);
	if (content->do_expansion)
	{
		if (raw_len <= LEXERKEEP$)
			return (err(ERRMSG_NULL_EXPAN), ERR_ARGS);
		ft_memcpy(buf, content->raw + LEXERKEEP$, raw_len - LEXERKEEP$);
		res = _do_arg_ops(s, content, buf, &value);
		if (0 != res)
			return (res);
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

	argl = NULL;
	if (a->type != AST_NODE_CMD)
		return (ERR_INVALID_CMD_TYPE);
	argl = p_get_args(a);
	if (*argl)
	{
		if (true == a->data.cmd.do_expansion)
		{
			res = lstiter_state(s, *argl, _p_do_arg_expansion);
			if (0 != res)
				return (res);
		}
		if (true == a->data.cmd.do_globbing)
		{
			debug_print(DEBUGMSG_ARGP_PRE_G, argl, *argl); // TODO remove
			ft_lstiter_ins_rwd(argl, p_do_globbing_args);
			debug_print(DEBUGMSG_ARGP_POST_G, argl, *argl); // TODO remove
		}
		a->data.cmd.argc = ft_lstsize(*argl);
		debug_print_list(*argl);
		*args = list_to_array(*argl, a->data.cmd.argc);
		if (NULL == *args)
			return (ERR_MEM);
	}
	return (0);
}
