/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lex_init.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rpeavey <rpeavey@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/10 16:32:32 by rpeavey           #+#    #+#             */
/*   Updated: 2025/04/10 16:32:33 by rpeavey          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "lex_int.h"

t_lex_fns	*init_lex_fns(t_state *s)
{
	const t_mem_mgr	*m = get_mem(s);
	t_lex_fns		*fns;

	fns = m->f(&((t_mem_mgr *)m)->list, sizeof(t_lex_fns));
	if (!fns)
		exit_clean(&((t_mem_mgr *)m)->list, ENOMEM, __FUNCTION__, EMSG_MALLOC);
	fns->lex_get_eof = lex_get_eof;
	fns->lex_get_lines = lex_get_lines;
	fns->lex_match_heredoc = match_heredoc;
	fns->write_heredoc = write_heredoc;
	fns->read_heredoc = read_heredoc;
	return (fns);
}

static inline void	_init_lexer_state(t_state *state, t_lex *lexer,
		int start_state)
{
	lexer->global_state = state;
	if (NORMAL == start_state)
		lexer->tokenizer = tokenize_normal;
	if (IN_SINGLE_QUOTES == start_state)
		lexer->tokenizer = tokenize_single_quotes;
	if (IN_DOUBLE_QUOTES == start_state)
	{
		lexer->tokenizer = tokenize_double_quotes;
		lexer->do_wordsplit = false;
		st_int_push(lexer->stack, IN_DOUBLE_QUOTES);
	}
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
	lexer->do_wordsplit = true;
	lexer->input_incomplete = false;
	lexer->is_subtoken = false;
	lexer->is_assignment = false;
	lexer->lex_err = 0;
}

t_lex	*create_lexer(t_state *state, int start_state, const char *s)
{
	t_lex	*lexer;

	dprint(_MOD_ ": %s\n", __FUNCTION__);
	lexer = myalloc(&(get_mem(state)->list), sizeof(t_lex));
	if (lexer)
	{
		_init_lexer_flags(lexer);
		lexer->stack = st_int_create(get_mem(state));
		lexer->raw_string = s;
		lexer->ptr = (char *)s;
		lexer->token_list = NULL;
		lexer->eof_word = NULL;
		lexer->lines = 0;
		lexer->last_grp_tok = NULL;
		lexer->tokc = 0;
		lexer->keep_dollar = LEXKEEPDOLLR;
		register_lexer_destroy(state, destroy_lexer);
		register_token_destroy(state, destroy_token);
		if (false == _allocate_buf_and_hasht(get_mem(state), lexer))
			exit_clean(&get_mem(state)->list, ENOMEM, __FUNCTION__,
				EMSG_MALLOC);
		build_hasht(get_mem(state), lexer);
		_init_lexer_state(state, lexer, start_state);
	}
	return (lexer);
}
