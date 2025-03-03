#include "token_int.h"

int	tok_print(void *content)
{
	t_tok	*tok;

	tok = (t_tok *)content;
	if (tok)
	{
		if (GROUP == tok->class)
		{
			log_print(_MOD_": Grp_tok: (grp) Exp: - Glb: - Sub: - tokc: %d \n", tok->t.meta.tokc);
			if (0 != ft_lstiter(tok->t.meta.tokens, tok_print))
				err("LSTITER");
		}
		else if (tok_get_issubtoken(tok))
			log_print(_MOD_": Sub_tok: %02d Exp: %d Glb: %d Sub: %d Val: %s \n", tok->t.tok.type,
			tok->t.tok.do_expansion, tok->t.tok.do_globbing, tok->t.tok.is_subtoken, tok->t.tok.raw);	
		else		
			log_print(_MOD_": Token: %02d Exp: %d Glb: %d Sub: %d Val: %s \n", tok->t.tok.type,
			tok->t.tok.do_expansion, tok->t.tok.do_globbing, tok->t.tok.is_subtoken, tok->t.tok.raw);	
	}
	return (0);
}

char	*tok_get_raw(t_tok *token)
{
	if (GROUP == token->class)
	{
		return (NULL);
	}
	return (token->t.tok.raw);
}

enum e_tok_type	tok_get_type(t_tok *token)
{
	if (GROUP == token->class)
	{
		return (TOK_GROUP_WORD);
	}
	return (token->t.tok.type);
}

// Works on a list of TOKENS
int	tok_do_grp_combine(t_state *s, void *c)
{
	const t_tok	*content = (t_tok *)c;
	static char			*str = "";
	char				*tmp;

	debug_print("%s got %s and %s\n", __FUNCTION__, str, content->t.tok.raw);
	tmp = ft_strjoin(str, content->t.tok.raw);
	if (tmp)
	{
		if (ft_strlen(str) > 0)
			free(str);
		str = tmp;
		set_tmp(s, str);
		debug_print("%s returning %s\n", __FUNCTION__, get_tmp(s));
		return (0);
	}
	set_error(s, ERR_MEM);
	return (ERR_MEM);
}



/* Looks for env values of key loaded in buf */
static int	_do_tok_ops(t_state *s, const t_tok *c, char *buf,
	char **value)
{
char	*new_raw;

if (check_special_expansions(s, buf, value) < 0)
{
	if (*value)
	{
		free(c->t.tok.raw);
		((t_tok *)c)->t.tok.raw = *value;
	}
}
else
{
	*value = get_sh_env(s, buf);
	if (*value)
	{
		new_raw = ft_strdup(*value);
		if (!new_raw)
			return (err("MALLOC\n"), ERR_MEM);
		free(c->t.tok.raw);
		((t_tok *)c)->t.tok.raw = new_raw;
	}
}
return (0);
}


int	tok_do_expansion(t_state *s, void *c)
{
	char				*value;
	const t_tok			*content = (t_tok *)c;
	int					res;
	char				buf[MAX_ENVVAR_LEN];
	size_t				raw_len;

	res = 0;
	value = NULL;
	if (GROUP == content->class)
		return (0);
	raw_len = ft_strnlen(content->t.tok.raw, MAX_ENVVAR_LEN);
	ft_memset(buf, 0, sizeof(buf));
	debug_print(_MOD_ "%s: %s\n", __FUNCTION__, content->t.tok.raw);
	if (content->t.tok.do_expansion)
	{
		if (raw_len <= lex_get_keepd(get_lexer(s)))
			return (err("error"), ERR_ARGS);
		ft_memcpy(buf, content->t.tok.raw + lex_get_keepd(get_lexer(s)), raw_len - lex_get_keepd(get_lexer(s)));
		res = _do_tok_ops(s, content, buf, &value);
		if (0 != res)
			return (res);
		debug_print(_MOD_ "%s: outtie: %s\n", __FUNCTION__, value);
	}
	return (0);
}
