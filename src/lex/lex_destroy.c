#include "lex_int.h"

/* Doesn't free raw_string */
void	destroy_lexer(t_mem_mgr *m, void **instance)
{
	t_lex	*lexer;

	lexer = (t_lex *)(*instance);
	dprint(_MOD_ ": %s...\n", __FUNCTION__);
	if (!lexer)
		return ;
	if (lexer->buf)
		m->dealloc(&m->list, lexer->buf);
	if (lexer->eof_word)
		m->dealloc(&m->list, lexer->eof_word);
	if (lexer->hasht)
		ht_destroy(m, lexer->hasht, lex_destroy_ht_node);
	if (lexer->token_list)
		ft_lstclear_tmp(m, &lexer->token_list, destroy_token);
	m->dealloc(&m->list, lexer);
	*instance = NULL;
	dprint(_MOD_ ": %s...DONE\n", __FUNCTION__);
}
