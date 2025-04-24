
#include "token_int.h"

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
