#include "parse_int.h"

#define EMSG_PATH_MALLOC "Allocation for path value failed.\n"
#define EMSG_NULL_EXPAN "Null expansion variable.\n"

/* Looks for env values of key loaded in buf */
static int	_do_arg_ops(t_state *s, const t_arg_data *c, char *buf,
		char **value)
{
	char	*new_raw;

	new_raw = NULL;
	if (check_special_expansions(s, buf, value) < 0)
	{
		if (*value)
		{
			(get_mem(s))->dealloc(&get_mem(s)->list, c->raw);
			((t_arg_data *)c)->raw = *value;
		}
	}
	else
	{
		*value = get_env_val(s, buf);
		if (*value)
		{
			new_raw = ft_strdup_tmp(get_mem(s), *value);
			if (!new_raw)
				exit_clean(&get_mem(s)->list, ENOMEM, __FUNCTION__,
					EMSG_PATH_MALLOC);
		}
		(get_mem(s))->dealloc(&get_mem(s)->list, c->raw);
		((t_arg_data *)c)->raw = new_raw;
	}
	return (0);
}

/* Passed to t_ast_node.cmd's t_list*args iterator.
 * If flag set, looks for expansion values in shell envp
 * by passing a buffer loaded with the key and replaces
 * t_arg_data.raw string, omitting any '$'.
 * The string to expand must be more than LEXKEEPDOLLR char (for '$').
 */
int	p_do_arg_expansion(t_state *s, void *c)
{
	char				*value;
	const t_arg_data	*content = (t_arg_data *)c;
	int					res;
	char				buf[INPUT_BUF_SZ];
	size_t				raw_len;

	res = 0;
	value = NULL;
	if (NULL == c)
		return (ERR_ARGS);
	raw_len = ft_strnlen(content->raw, INPUT_BUF_SZ);
	ft_memset(buf, 0, sizeof(buf));
	dprint(PDMSG_IN, _MOD_, __FUNCTION__, content->raw);
	if (content->do_expansion)
	{
		if (raw_len <= LEXKEEPDOLLR)
			return (err(EMSG_NULL_EXPAN), ERR_ARGS);
		ft_memcpy(buf, content->raw + LEXKEEPDOLLR, raw_len - LEXKEEPDOLLR);
		res = _do_arg_ops(s, content, buf, &value);
		if (res != 0)
			return (res);
		dprint(PDMSG_OUT, _MOD_, __FUNCTION__, value);
	}
	return (0);
}
