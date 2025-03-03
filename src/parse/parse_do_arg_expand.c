#include "parse_int.h"

#define ERRMSG_PATH_MALLOC "Allocation for path value failed.\n"
#define ERRMSG_NULL_EXPAN "Null expansion variable.\n"

#define DEBUGMSG_ARGP_PRE_G _MOD_ ": Pre-glob headp: %p, 1st node: %p\n"
#define DEBUGMSG_ARGP_POST_G _MOD_ ": Post-glob headp: %p, 1st node: %p\n"

#define DMSG_IN _MOD_ ": %s: got: %s\n"
#define DMSG_OUT _MOD_ ": %s: found: %s\n"

/* Add special $? codes here.
 * buf will start with char after any '$'.
 * Returns the negated length of the expansion found (minus $).
 * Or error codes.
 * Places the expanded heap string in *value
 */
int	check_special_expansions(t_state *s, const char *buf, char **value)
{
	const int	*status = get_status(s);

	if (!buf || !value)
		return (ERR_ARGS);
	debug_print(DMSG_IN, __FUNCTION__, buf);
	if (ft_strcmp(buf, "?") == 0)
	{
		if (status == NULL)
			*value = ft_itoa(0);
		else
			*value = ft_itoa(*status);
		if (*value == NULL)
			return (ERR_MEM);
		debug_print(DMSG_OUT, __FUNCTION__, *value);
		return (-1);
	}
	return (ERR_GENERAL);	
}

/* Looks for env values of key loaded in buf */
static int	_do_arg_ops(t_state *s, const t_arg_data *c, char *buf,
		char **value)
{
	char	*new_raw;

	if (check_special_expansions(s, buf, value) < 0)
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
	if (NULL == c)
		return (ERR_ARGS);
	raw_len = ft_strnlen(content->raw, MAX_ENVVAR_LEN);
	ft_memset(buf, 0, sizeof(buf));
	debug_print(DMSG_IN, __FUNCTION__, content->raw);
	if (content->do_expansion)
	{
		if (raw_len <= LEXERKEEP$)
			return (err(ERRMSG_NULL_EXPAN), ERR_ARGS);
		ft_memcpy(buf, content->raw + LEXERKEEP$, raw_len - LEXERKEEP$);
		res = _do_arg_ops(s, content, buf, &value);
		if (0 != res)
			return (res);
		debug_print(DMSG_OUT, __FUNCTION__, value);
	}
	return (0);
}

// runs through llist and calls iterator on any group token llists
static int	_p_do_grparg_processing(t_state *s, void *c)
{
	t_arg_data	*content;
	t_list		**lst;
	int			res;

	res = 0;
	content = (t_arg_data *)c;
	if (NULL == c)
		return (0);
	if (false == content->is_grouparg)
		return (0);
	debug_print("%s: got list: %p exp:%d glob:%d\n", __FUNCTION__, content->lst_tokens, content->do_expansion, content->do_globbing);
	if (content->do_expansion)
	{
		res = lstiter_state(s, content->lst_tokens, tok_do_expansion);
		if (0 != res)
			return (res);
	}
	if (content->do_globbing)
	{
		lst = &content->lst_tokens;
		ft_lstiter_ins_rwd(lst, p_do_globbing_args);
	}
	res = lstiter_state(s, content->lst_tokens, tok_do_grp_combine);
	if (0 == res)
		content->raw = ft_strdup(get_tmp(s)); //TODO malloc
	debug_print("%s: returning str: %s\n", __FUNCTION__, content->raw);
	return (res);
}

/* Replaces any expansions and inserts any globbing
 * results into args list, converts to array
 * and returns that array via args ptr.
 */
int	p_do_arg_processing(t_state *s, t_ast_node *a, char ***args)
{
	t_list	**argl;
	int		res;

	argl = NULL;
	res = 0;
	if (a->type != AST_NODE_CMD)
		return (ERR_INVALID_CMD_TYPE);
	argl = p_get_args(a);
	if (*argl)
	{
		if (a->data.cmd.has_grouptoks)
			res = lstiter_state(s, *argl, _p_do_grparg_processing);
		else // delete
		{
			if (a->data.cmd.do_expansion)
				res = lstiter_state(s, *argl, _p_do_arg_expansion);
			if (a->data.cmd.do_globbing)
			{
				debug_print(DEBUGMSG_ARGP_PRE_G, argl, *argl); // TODO remove
				ft_lstiter_ins_rwd(argl, p_do_globbing_args);
				debug_print(DEBUGMSG_ARGP_POST_G, argl, *argl); // TODO remove
			}
		}
		if (0 != res)
			return (res);
		a->data.cmd.argc = ft_lstsize(*argl);
		debug_print_list(*argl);
		*args = list_to_array(*argl, a->data.cmd.argc);
		if (NULL == *args)
			return (ERR_MEM);
	}
	return (0);
}
