#include "token_int.h"

static void	_init_group_token(t_tok *token, size_t pos)
{
	token->class = GROUP;
	token->t.meta.pos = pos;
	token->t.meta.tokc = 0;
	token->t.meta.do_expansion = false;
	token->t.meta.do_globbing = false;
	token->t.meta.tokens = NULL;
}

static int	_init_normal_token(t_state *st, t_tok *token, const char *s, int type, size_t pos)
{
	if (ft_strlen(s) > MAX_RAW_INPUT_LEN)
		return (err(_MOD_": raw input buf overflow\n"), free(token), ERR_BUFFLOW);
	token->class = NORMAL;
	token->t.tok.raw = ft_strdup(s); //TODO, wrap this
	token->t.tok.type = type;
	token->t.tok.pos = pos;
	token->t.tok.is_subtoken = false;
	token->t.tok.do_expansion = false;
	token->t.tok.in_dquotes = false;
	token->t.tok.do_globbing = false;
	if (!token->t.tok.raw)
	{
		free(token);
		return (err("MALLOC\n"), ERR_MEM);
	}
	return (0);
}

t_tok	*create_token(t_state *st, const char *s, int type, size_t pos)
{
	t_tok	*token;

	if (!s)
		return (NULL);
	token = myalloc(st, sizeof(t_tok));
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
			_init_normal_token(st, token, s, type, pos);
		}
	}
	return (token);
}

/* void star due to linked list destroy method */
void	destroy_token(t_state *s, void *token)
{
	t_tok *t;
	
	t = (t_tok *)token;
	if (!t)
		return ;
	if (GROUP == t->class)
		ft_lstclear_tmp(s, &t->t.meta.tokens, destroy_token);
	else
	{
		if (t->t.tok.raw)
			free(t->t.tok.raw);
	}
	myfree(s, t);
	token = NULL;
}

// Called by parser to deep copy tokens and keep free routines separate
void	*copy_token(t_state *st, const void *tok)
{
	t_tok	*t;
	t_tok	*new_t;

	t = (t_tok *)tok;
	new_t = myalloc(st, sizeof(t_tok));
	if (new_t == NULL)
		return (err(EMSG_MALLOC), NULL);
	new_t->class = t->class;
	if (GROUP == t->class)
	{
		new_t->t.meta.pos = t->t.meta.pos;
		new_t->t.meta.tokc = t->t.meta.tokc;
		new_t->t.meta.do_expansion = t->t.meta.do_expansion;
		new_t->t.meta.do_globbing = t->t.meta.do_globbing;
		new_t->t.meta.tokens = ft_lstcopy_tmp(st, t->t.meta.tokens, copy_token, destroy_token); //TODO handle bad mallco
	}
	else 
	{
		new_t->t.tok.raw = ft_strdup(t->t.tok.raw);
		if (NULL == new_t->t.tok.raw)
			return (myfree(st, new_t), err(EMSG_MALLOC), NULL);
		new_t->t.tok.type = t->t.tok.type;
		new_t->t.tok.pos = t->t.tok.pos;
		new_t->t.tok.do_globbing = t->t.tok.do_globbing;
		new_t->t.tok.do_expansion = t->t.tok.do_expansion;
	}
	return (new_t);
}
