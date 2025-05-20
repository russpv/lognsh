#include "parse_int.h"

#define CONT 10
#define ZEROLEN 9

static void	_commit_body(t_state *s, t_redir_data *r, char *new_body)
{
	(get_mem(s))->dealloc(&get_mem(s)->list, r->heredoc_body);
	r->heredoc_body = ft_strdup_tmp(get_mem(s), new_body);
	if (NULL == r->heredoc_body)
		exit_clean(&get_mem(s)->list, ENOMEM, __FUNCTION__, EMSG_MALLOC);
}

static int	_get_new_val(t_state *s, char *buf, char **new_val)
{
	int	res;

	if (!s || !buf || !new_val)
		return (ERR_ARGS);
	res = check_special_expansions(s, buf, new_val);
	if (res > 0)
		return (res);
	else if (res == 0)
		*new_val = get_env_val(s, buf);
	if (ft_strnlen(*new_val, MAX_INPUT_SZ) > INPUT_BUF_SZ)
		return (ERR_BUFFLOW);
	return (0);
}

// Checks envp for key buf, wipes key, inserts value, jumps ptr
static int	_insert_expanded_var(t_state *s, char *buf, char **ptr,
		t_redir_data *r)
{
	char	*new_val;
	char	new_body[MAX_INPUT_SZ];
	size_t	offset;
	int		res;

	new_val = NULL;
	dprint("%s: %s: got:_%s_\n", _MOD_, __FUNCTION__, buf);
	res = _get_new_val(s, buf, &new_val);
	if (0 != res)
		return (res);
	offset = *ptr - r->heredoc_body;
	if (ft_strnlen(r->heredoc_body, INPUT_BUF_SZ) + ft_strlen(new_val)
		+ 1 > MAX_INPUT_SZ)
		return (pbufflow(NULL), ERR_BUFFLOW);
	ft_strlcpy(new_body, r->heredoc_body, offset);
	offset += ft_strlen(new_val);
	ft_strlcat(new_body, new_val, mymax(MAX_INPUT_SZ - offset, 0));
	ft_strlcat(new_body, *ptr + ft_strlen(buf), mymax(MAX_INPUT_SZ - offset
			- ft_strlen(*ptr + ft_strlen(buf)), 0));
	_commit_body(s, r, new_body);
	*ptr = r->heredoc_body + offset - 1;
	return (0);
}

static int	_do_expansion(t_state *s, t_redir_data *r, char *buf, char *ptr)
{
	int	len;

	if ((unsigned char)OP_ENV == *ptr)
	{
		len = ft_varnamelen((const char *)(ptr + 1));
		len |= (int)(check_special_expansions(s, ft_memcpy(buf, ptr + 1, 1),
					NULL) < 0);
		if (len <= 0 || len >= MAX_NAME_LEN)
			return (ZEROLEN);
		++ptr;
		ft_memcpy(buf, ptr, len);
		if (0 != _insert_expanded_var(s, buf, &ptr, r))
			return (ERR_MEM);
		ft_memset(buf, 0, len);
		return (ZEROLEN);
	}
	return (0);
}

// Either current char is literal, escape, or expansion var
// Never accepts first backslash.
int	p_do_heredoc_expansion(t_state *s, t_redir_data *r)
{
	char	*ptr;
	char	buf[MAX_NAME_LEN];
	int		res;

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
		res = _do_expansion(s, r, buf, ptr);
		if (ERR_MEM == res)
			return (res);
		else if (ZEROLEN != res && CONT != res)
			if ((unsigned char)TK_ESC == *ptr)
				ft_memmove(ptr, ptr + 1, ft_strlen(ptr));
		ptr++;
	}
	return (0);
}
