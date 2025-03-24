#include "lex_int.h"

static inline void	_init_lexer_state(t_lex *lexer, int start_state)
{
	if (NORMAL == start_state)
		lexer->tokenizer = tokenize_normal;
	if (IN_SINGLE_QUOTES == start_state)
		lexer->tokenizer = tokenize_single_quotes;
	if (IN_DOUBLE_QUOTES == start_state)
		lexer->tokenizer = tokenize_double_quotes;
	lexer->state = start_state;
}

static inline bool	_allocate_buf_and_hasht(t_mem_mgr *m, t_lex *lexer)
{
	lexer->buf = myalloc(&m->list, sizeof(char) * LEX_BUFSZ);
	if (!lexer->buf)
		exit_clean(&m->list, ENOMEM, __FUNCTION__, EMSG_MALLOC);
	ft_memset(lexer->buf, 0, LEX_BUFSZ);
	lexer->buf_idx = 0;
	lexer->hasht = ht_create(m);
	if (!lexer->hasht)
		exit_clean(&m->list, ENOMEM, __FUNCTION__, EMSG_MALLOC);
	return (lexer->hasht != NULL);
}

static inline void	_init_lexer_flags(t_lex *lexer)
{
	lexer->do_heredoc_expansion = true;
	lexer->escape_mode = false;
	lexer->do_expansion = INITVAL;
	lexer->do_globbing = INITVAL;
	lexer->do_wordsplit = 1;
	lexer->input_incomplete = false;
	lexer->is_subtoken = false;
}

t_lex	*create_lexer(t_state *state, int start_state, const char *s)
{
	t_lex	*lexer;

	debug_print(_MOD_ ": %s\n", __FUNCTION__);
	lexer = myalloc(&(get_mem(state)->list), sizeof(t_lex));
	if (lexer)
	{
		_init_lexer_state(lexer, start_state);
		_init_lexer_flags(lexer);
		lexer->stack = st_int_create(get_mem(state));
		lexer->raw_string = s;
		lexer->ptr = (char *)s;
		lexer->token_list = NULL;
		lexer->eof_word = NULL;
		lexer->last_grp_tok = NULL;
		lexer->tokc = 0;
		lexer->keep_dollar = LEXERKEEP$;
		register_lexer_destroy(state, destroy_lexer);
		register_token_destroy(state, destroy_token);
		if (false == _allocate_buf_and_hasht(get_mem(state), lexer))
			exit_clean(&get_mem(state)->list, ENOMEM, __FUNCTION__,
				EMSG_MALLOC);
		build_hasht(get_mem(state), lexer);
	}
	return (lexer);
}

/* Doesn't free raw_string */
void	destroy_lexer(t_mem_mgr *m, void *instance)
{
	t_lex		*lexer;

	lexer = (t_lex *)instance;
	debug_print(_MOD_ ": %s...\n", __FUNCTION__);
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
}
