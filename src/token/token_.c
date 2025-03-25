#include "token_int.h"

static void	_init_group_token(t_tok *token, size_t pos)
{
	token->class = GROUP;
	token->t.meta.pos = pos;
	token->t.meta.tokc = 0;
	token->t.meta.in_dquotes = false;
	token->t.meta.do_expansion = false;
	token->t.meta.do_globbing = false;
	token->t.meta.tokens = NULL;
}

static int	_init_normal_token(t_mem_mgr *mgr, t_tok *token, const char *s, int type, size_t pos)
{
	if (ft_strnlen(s, 4096) > MAX_RAW_INPUT_LEN)
		return (err(_MOD_": raw input buf overflow\n"), mgr->dealloc(&mgr->list, token), ERR_BUFFLOW);
	token->class = NORMAL;
	token->t.tok.raw = ft_strdup_tmp(mgr, s);
	token->t.tok.type = type;
	token->t.tok.pos = pos;
	token->t.tok.is_subtoken = false;
	token->t.tok.do_expansion = false;
	token->t.tok.in_dquotes = false;
	token->t.tok.do_globbing = false;
	token->t.tok.is_combinable = true;
	if (!token->t.tok.raw)
		exit_clean(&mgr->list, ENOMEM, __FUNCTION__, EMSG_MALLOC);
	return (0);
}

t_tok	*create_token(t_mem_mgr *mgr, const char *s, int type, size_t pos)
{
	t_tok	*token;

	if (!s)
		return (NULL);
	token = mgr->f(&mgr->list, sizeof(t_tok));
	if (token)
	{
		if (type == TOK_GROUP_WORD)
		{
			debug_print(_MOD_": %s: (grp) typ_%d \n", __FUNCTION__, type);
			_init_group_token(token, pos);
		}
		else
		{
			debug_print(_MOD_": %s: %s_ typ_%d \n", __FUNCTION__, s, type);
			_init_normal_token(mgr, token, s, type, pos);
		}
	}
	return (token);
}

// Creates a dummy token, non-combinable
void	*create_tmp_token(t_mem_mgr *mgr, const void *s)
{
	t_tok	*token;

	if (!s)
		return (NULL);
	token = mgr->f(&mgr->list, sizeof(t_tok));
	if (token)
	{
		debug_print(_MOD_": %s: %s_ typ_%d \n", __FUNCTION__, (const char *)s, TOK_WORD);
		_init_normal_token(mgr, token, (const char *)s, TOK_WORD, -1);
		token->t.tok.is_combinable = false;
	}
	return (token);
}
