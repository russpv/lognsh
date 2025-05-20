#include "tokens_int.h"

#define DMSG_CT ": %s: %s_ typ_%02d len_%ld\n"

/* void star star due to linked list destroy method */
void	destroy_token(t_mem_mgr *mgr, void **token)
{
	t_tok	*t;

	if (!token)
		return ;
	if (!(*token))
		return ;
	t = (t_tok *)(*token);
	if (GROUP == t->class)
		ft_lstclear_tmp(mgr, &t->t.meta.tokens, destroy_token);
	else
	{
		if (t->t.tok.raw)
			mgr->dealloc(&mgr->list, t->t.tok.raw);
	}
	mgr->dealloc(&mgr->list, t);
	*token = NULL;
}

static void	_init_group_token(t_tok *token, size_t pos, int type)
{
	dprint(_MOD_ ": %s: (grp) typ_%02d \n", __FUNCTION__, type);
	token->class = GROUP;
	token->t.meta.pos = pos;
	token->t.meta.tokc = 0;
	token->t.meta.in_dquotes = false;
	token->t.meta.do_expansion = false;
	token->t.meta.do_globbing = false;
	token->t.meta.tokens = NULL;
}

static int	_init_normal_token(t_tok *token, int type, size_t pos)
{
	token->class = NORMAL;
	token->t.tok.type = type;
	token->t.tok.pos = pos;
	token->t.tok.is_subtoken = false;
	token->t.tok.do_expansion = false;
	token->t.tok.in_dquotes = false;
	token->t.tok.do_globbing = false;
	token->t.tok.is_combinable = true;
	return (0);
}

t_tok	*create_token(t_mem_mgr *mgr, const char *s, int type, size_t pos)
{
	t_tok	*token;

	if (!mgr)
		return (NULL);
	token = mgr->f(&mgr->list, sizeof(t_tok));
	if (token)
	{
		if (type == TOK_GROUP_WORD)
			_init_group_token(token, pos, type);
		else
		{
			token->t.tok.raw = NULL;
			token->t.tok.raw_len = ft_strnlen(s, MAX_INPUT_SZ);
			dprint(_MOD_ DMSG_CT, __FUNCTION__, s, type, token->t.tok.raw_len);
			if (token->t.tok.raw_len > MAX_INT_BUFLEN)
				return (pbufflow(s), mgr->dealloc(&mgr->list, token), NULL);
			if (token->t.tok.raw_len == 0 && TOK_NAME == type \
&& TOK_WORD == type)
				type = TOK_EMPTY;
			else
				token->t.tok.raw = ft_strdup_tmp(mgr, s);
			_init_normal_token(token, type, pos);
		}
	}
	return (token);
}

// Creates a simple token, non-combinable
// Signals no further processing needed
void	*create_tmp_token(t_mem_mgr *mgr, const void *s)
{
	t_tok	*token;

	if (!s)
		return (NULL);
	token = mgr->f(&mgr->list, sizeof(t_tok));
	if (token)
	{
		dvprint(_MOD_ ": %s: %s_ typ_%d \n", __FUNCTION__, (const char *)s,
			TOK_WORD);
		if (ft_strnlen(s, MAX_INPUT_SZ) > MAX_INT_BUFLEN)
			return (pbufflow(s), mgr->dealloc(&mgr->list, token), NULL);
		token->t.tok.raw = ft_strdup_tmp(mgr, s);
		if (!token->t.tok.raw)
			exit_clean(&mgr->list, ENOMEM, __FUNCTION__, EMSG_MALLOC);
		_init_normal_token(token, TOK_WORD, -1);
		token->t.tok.is_combinable = false;
	}
	return (token);
}

// Creates a dummy token as result of expansion
// Signals combiner to combine only token list boundaries
void	*create_split_token(t_mem_mgr *mgr, const void *s)
{
	t_tok	*token;

	if (!s)
		return (NULL);
	token = mgr->f(&mgr->list, sizeof(t_tok));
	if (token)
	{
		dprint(_MOD_ ": %s: %s_ typ_%d \n", __FUNCTION__, (const char *)s,
			TOK_WORD);
		if (ft_strnlen(s, MAX_INPUT_SZ) > MAX_INT_BUFLEN)
			return (pbufflow(s), mgr->dealloc(&mgr->list, token), NULL);
		token->t.tok.raw = ft_strdup_tmp(mgr, s);
		if (!token->t.tok.raw)
			exit_clean(&mgr->list, ENOMEM, __FUNCTION__, EMSG_MALLOC);
		_init_normal_token(token, TOK_WORD, -1);
		token->t.tok.is_combinable = false;
		token->t.tok.do_expansion = true;
	}
	return (token);
}


/* -------------------------------------------------------------------------
 * Function Group: Token copy logic
 * Description   : 
 * ------------------------------------------------------------------------- */

int	tok_add_subtok(t_mem_mgr *mgr, t_tok *grp, t_tok *sub)
{
	if (!grp || !sub)
		return (ERR_ARGS);
	ft_lstadd_back(&grp->t.meta.tokens, ft_lstnew_tmp(mgr, sub));
	return (0);
}

static void	_init_meta(t_mem_mgr *mgr, t_tok *t, t_tok *new_t)
{
	new_t->class = t->class;
	new_t->t.meta.pos = t->t.meta.pos;
	new_t->t.meta.tokc = t->t.meta.tokc;
	new_t->t.meta.do_expansion = t->t.meta.do_expansion;
	new_t->t.meta.do_globbing = t->t.meta.do_globbing;
	new_t->t.meta.tokens = ft_lstcopy_tmp(mgr, t->t.meta.tokens, copy_token,
			destroy_token);
}

static void	_init_norm(t_mem_mgr *mgr, t_tok *t, t_tok *new_t)
{
	new_t->class = t->class;
	new_t->t.tok.raw = ft_strdup_tmp(mgr, t->t.tok.raw);
	if (NULL == new_t->t.tok.raw)
		exit_clean(&mgr->list, ENOMEM, __FUNCTION__, EMSG_MALLOC);
	new_t->t.tok.type = t->t.tok.type;
	new_t->t.tok.pos = t->t.tok.pos;
	new_t->t.tok.do_globbing = t->t.tok.do_globbing;
	new_t->t.tok.do_expansion = t->t.tok.do_expansion;
	new_t->t.tok.is_combinable = t->t.tok.is_combinable;
	new_t->t.tok.is_subtoken = t->t.tok.is_subtoken;
	new_t->t.tok.in_dquotes = t->t.tok.in_dquotes;
}

// Called by parser to deep copy tokens and keep free routines separate
void	*copy_token(t_mem_mgr *mgr, const void *tok)
{
	t_tok	*t;
	t_tok	*new_t;

	if (!mgr || !tok)
		return (NULL);
	t = (t_tok *)tok;
	new_t = mgr->f(&mgr->list, sizeof(t_tok));
	if (new_t == NULL)
		exit_clean(&mgr->list, ENOMEM, __FUNCTION__, EMSG_MALLOC);
	if (GROUP == t->class)
		_init_meta(mgr, t, new_t);
	else
		_init_norm(mgr, t, new_t);
	dvprint(_MOD_ ": %s: Copied token _%s_ @ _%ld_\n", __FUNCTION__,
		tok_get_raw(new_t), tok_get_pos(new_t));
	return (new_t);
}
