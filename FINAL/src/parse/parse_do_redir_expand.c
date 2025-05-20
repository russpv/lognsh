#include "parse_int.h"

#define EMSG_PATH_MALLOC "Allocation for path value failed.\n"
#define EMSG_NULL_EXPAN "Null expansion variable.\n"

#define MSG_IN  "%s: p_do_expansion got: %s\n"
#define MSG_OUT "%s: p_do_expansion found: %s\n"

// Note: would be good to harden this
int	mymax(int one, int two)
{
	if (one > two)
		return (one);
	return (two);
}

/* Looks for env values of key loaded in buf */
static int	_get_expanded_fn(t_state *s, const t_redir_data *r, char *buf,
	char **value)
{
	char	*new_fn;

	if (check_special_expansions(s, buf, value) < 0)
	{
		if (*value)
		{
			(get_mem(s))->dealloc(&get_mem(s)->list, r->filename);
			((t_redir_data *)r)->filename = *value;
		}
	}
	else
	{
		*value = get_env_val(s, buf);
		if (*value)
		{
			new_fn = ft_strdup_tmp(get_mem(s), *value);
			if (!new_fn)
				exit_clean(&get_mem(s)->list, ENOMEM, __FUNCTION__,
					EMSG_PATH_MALLOC);
			(get_mem(s))->dealloc(&get_mem(s)->list, r->filename);
			((t_redir_data *)r)->filename = new_fn;
		}
	}
	return (0);
}

static int	_helper(t_state *s, const t_redir_data	*r_data, char *buf, \
size_t fn_len)
{
	char	*value;
	int		res;

	if (fn_len <= LEXKEEPDOLLR)
		return (err(EMSG_NULL_EXPAN), ERR_ARGS);
	ft_memcpy(buf, r_data->filename + LEXKEEPDOLLR, fn_len - LEXKEEPDOLLR);
	res = _get_expanded_fn(s, r_data, buf, &value);
	if (0 != res)
		return (res);
	dprint(MSG_OUT, _MOD_, value);
	return (0);
}

/* Passed to t_ast_node.cmd's t_list*args iterator.
 * If flag set, looks for expansion values in shell envp
 * by passing a buffer loaded with the key and replaces
 * t_arg_data.raw string, omitting any '$'.
 * The string to expand must be more than LEXKEEPDOLLR char (for '$').
 * Handles heredocs.
 */
int	p_do_red_expansion(t_state *s, void *r)
{
	const t_redir_data	*r_data = (t_redir_data *)r;
	char				buf[MAX_NAME_LEN];
	size_t				fn_len;

	if (NULL == r || NULL == s)
		return (ERR_ARGS);
	if (NULL == r_data->filename)
		return (p_do_heredoc_expansion(s, (t_redir_data *)r));
	fn_len = ft_strnlen(r_data->filename, MAX_NAME_LEN);
	ft_memset(buf, 0, sizeof(buf));
	dprint(MSG_IN, _MOD_, r_data->filename);
	if (r_data->do_expansion)
	{
		_helper(s, r_data, buf, fn_len);
	}
	return (0);
}
