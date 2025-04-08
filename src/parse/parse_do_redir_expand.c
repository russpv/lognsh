#include "parse_int.h"
#include <stdio.h>

#define EMSG_PATH_MALLOC "Allocation for path value failed.\n"
#define EMSG_NULL_EXPAN "Null expansion variable.\n"

#define DEBUGMSG_REDIRP_ANNOUNCE _MOD_ ": p_do_redir_processing...\n"
#define DBGMSG_REDIRP_GOT _MOD_ ": p_do_redir_processing got: glob_%d exp_%d\n"

#define DEBUGMSG_DOEXP_ANNOUNCE _MOD_ ": p_do_expansion got: %s\n"
#define DEBUGMSG_DOEXP_RES _MOD_ ": p_do_expansion found: %s\n"

/* Looks for env values of key loaded in buf */
static int	_get_expanded_fn(t_state *s, const t_redir_data *r, char *buf,
		char **value)
{
	char	*new_fn;

	if (check_special_expansions(s, buf, value) < 0)
	{
		if (*value)
		{
			get_mem(s)->dealloc(&get_mem(s)->list, r->filename);
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
			get_mem(s)->dealloc(&get_mem(s)->list, r->filename);
			((t_redir_data *)r)->filename = new_fn;
		}
	}
	return (0);
}

//TODO HARDEN THIS
static int mymax(int one, int two)
{
	if (one > two)
		return (one);
	return (two);
}

// Checks envp for key buf, wipes key, inserts value
static int	_insert_expanded_var(t_state *s, char *buf, char **ptr,
		t_redir_data *r)
{
	char	*new_val;
	char	new_body[MAX_INPUT_SZ];
	size_t	offset;
	int		res;

	new_val = NULL;
	debug_print("%s: got:_%s\n", __FUNCTION__, buf);
	res = check_special_expansions(s, buf, &new_val);
	if (res > 0)
		return (res);
	if (res == 0)
		new_val = get_env_val(s, buf);
	if (ft_strnlen(new_val, MAX_INPUT_SZ) > INPUT_BUF_SZ)
		return (ERR_BUFFLOW);
	offset = *ptr - r->heredoc_body;
	if (ft_strnlen(r->heredoc_body, INPUT_BUF_SZ) + ft_strlen(new_val) + 1 > MAX_INPUT_SZ)
		return (print_bufflow(NULL), ERR_BUFFLOW);
	ft_strlcpy(new_body, r->heredoc_body, offset);
	offset += ft_strlen(new_val);
	ft_strlcat(new_body, new_val, mymax(MAX_INPUT_SZ - offset, 0));
	ft_strlcat(new_body, *ptr + ft_strlen(buf), mymax(MAX_INPUT_SZ - offset
		- ft_strlen(*ptr + ft_strlen(buf)), 0));
	get_mem(s)->dealloc(&get_mem(s)->list, r->heredoc_body);
	r->heredoc_body = ft_strdup_tmp(get_mem(s), new_body);
	if (NULL == r->heredoc_body)
		exit_clean(&get_mem(s)->list, ENOMEM, __FUNCTION__, EMSG_MALLOC);
	*ptr = r->heredoc_body + offset - 1;
	return (0);
}

static int	_p_do_heredoc_expansion(t_state *s, t_redir_data *r)
{
	char	*ptr;
	char	buf[MAX_NAME_LEN];
	int		len;

	ft_memset(buf, 0, sizeof(buf));
	if (NULL == r)
		return (ERR_ARGS);
	if (NULL == r->heredoc_body)
		return (ERR_ARGS);
	if (false == r->do_expansion)
		return (0);
	ptr = r->heredoc_body;
	while (*ptr)
	{
		if ((unsigned char)OP_ENV == *ptr)
		{
			len = ft_varnamelen((const char *)(ptr + 1));
			len |= (int)(check_special_expansions(s, ft_memcpy(buf, ptr + 1, 1),
						NULL) < 0);
			if (len > 0)
			{
				++ptr;
				ft_memcpy(buf, ptr, len);
				if (0 != _insert_expanded_var(s, buf, &ptr, r))
					return (ERR_MEM);
				ft_memset(buf, 0, len);
				continue ;
			}
		}
		else if ((unsigned char)TK_ESC == *ptr)
		{
			ft_memmove(ptr, ptr + 1, ft_strlen(ptr));
			ptr++;
			continue ;
		}
		ptr++;
	}
	return (0);
}

/* Passed to t_ast_node.cmd's t_list*args iterator.
 * If flag set, looks for expansion values in shell envp
 * by passing a buffer loaded with the key and replaces
 * t_arg_data.raw string, omitting any '$'.
 * The string to expand must be more than LEXERKEEP$ char (for '$').
 * Handles heredocs.
 */
int	p_do_red_expansion(t_state *s, void *r)
{
	char				*value;
	const t_redir_data	*r_data = (t_redir_data *)r;
	int					res;
	char				buf[MAX_NAME_LEN];
	size_t				fn_len;

	res = 0;
	value = NULL;
	if (NULL == r || NULL == s)
		return (ERR_ARGS);
	if (NULL == r_data->filename)
		return (_p_do_heredoc_expansion(s, (t_redir_data *)r));
	fn_len = ft_strnlen(r_data->filename, MAX_NAME_LEN);
	ft_memset(buf, 0, sizeof(buf));
	debug_print(DEBUGMSG_DOEXP_ANNOUNCE, r_data->filename);
	if (r_data->do_expansion)
	{
		if (fn_len <= LEXERKEEP$)
			return (err(EMSG_NULL_EXPAN), ERR_ARGS);
		ft_memcpy(buf, r_data->filename + LEXERKEEP$, fn_len - LEXERKEEP$);
		res = _get_expanded_fn(s, r_data, buf, &value);
		if (0 != res)
			return (res);
		debug_print(DEBUGMSG_DOEXP_RES, value);
	}
	return (0);
}
