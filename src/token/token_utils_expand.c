
#include "token_int.h"

static void	_check_envp(t_state *s, const t_tok *c, char *buf, char **value)
{
	char	*new_raw;

	new_raw = NULL;
	dvprint(_MOD_ ": checking envp value\n");
	*value = get_env_val(s, buf);
	if (*value)
	{
		new_raw = ft_strdup_mem(get_mem(s), *value);
		if (!new_raw)
			exit_clean(&get_mem(s)->list, ENOMEM, __FUNCTION__, EMSG_MALLOC);
	}
	myfree(&get_mem(s)->list, c->t.tok.raw);
	((t_tok *)c)->t.tok.raw = new_raw;
}

/* Looks for env values of key loaded in buf, replaces raw. */
static int	_do_tok_ops(t_state *s, const t_tok *c, char *buf, char **value)
{
	if (check_special_expansions(s, buf, value) < 0)
	{
		if (*value)
		{
			myfree(&get_mem(s)->list, c->t.tok.raw);
			((t_tok *)c)->t.tok.raw = *value;
		}
	}
	else
		_check_envp(s, c, buf, value);
	return (0);
}

static int	_do_expansion(t_state *s, const t_tok *content, char *value,
		size_t raw_len)
{
	char	keybuf[MAX_ENV_NAME_LEN + 1];
	int		res;

	res = 0;
	ft_memset(keybuf, 0, sizeof(keybuf));
	if (content->t.tok.do_expansion)
	{
		if (raw_len <= lex_get_keepd(get_lexer(s)))
			return (err("error"), ERR_ARGS);
		ft_memcpy(keybuf, content->t.tok.raw + lex_get_keepd(get_lexer(s)), raw_len
			- lex_get_keepd(get_lexer(s)));
		res = _do_tok_ops(s, content, keybuf, &value);
		if (0 != res)
			return (res);
		dprint(_MOD_ ": %s: expanded var: %s\n", __FUNCTION__, value);
	}
	return (res);
}

int	tok_do_expansion(t_state *s, void *c)
{
	char		*value;
	const t_tok	*content = (t_tok *)c;
	int			res;
	size_t		raw_len;

	res = 0;
	value = NULL;
	if (NULL == content)
		return (0);
	if (GROUP == content->class)
		return (0);
	raw_len = ft_strnlen(content->t.tok.raw, MAX_ENV_NAME_LEN);
	dprint(_MOD_ ": %s:  %s\n", __FUNCTION__, content->t.tok.raw);
	res = _do_expansion(s, content, value, raw_len);
	return (res);
}
