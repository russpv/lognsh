/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lex_state_REF_utils.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dayeo <dayeo@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/06 00:57:08 by dayeo             #+#    #+#             */
/*   Updated: 2025/04/06 00:57:24 by dayeo            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "lex_int.h"

// Stops tokenizer loop from running
// ON_EOF can be set only after NULL token is added
bool	_handle_eof_state(t_lex *lexer)
{
	if (ON_EOF == lexer->state)
	{
		debug_print(_MOD_ ": %s: Transitioning to DONE state\n", __FUNCTION__);
		lexer->state = DONE;
		return (true);
	}
	return (false);
}

bool	_handle_quote_state(t_lex *lexer)
{
	if ((unsigned char)*lexer->ptr == OP_DQUOTE
		|| IN_DOUBLE_QUOTES == st_int_peek(lexer->stack))
	{
		if ((unsigned char)*lexer->ptr == OP_DQUOTE
			&& IN_DOUBLE_QUOTES == st_int_peek(lexer->stack))
		{
			st_int_pop(lexer->stack);
			lexer->do_wordsplit = true;
			lexer->ptr++;
			return (false);
		}
		if (IN_DOUBLE_QUOTES != st_int_peek(lexer->stack))
			st_int_push(lexer->stack, IN_DOUBLE_QUOTES);
		lexer->state = IN_DOUBLE_QUOTES;
		lexer->do_wordsplit = false;
		lexer->tokenizer = tokenize_double_quotes;
		return (true);
	}
	if ((unsigned char)*lexer->ptr == OP_SQUOTE)
	{
		lexer->state = IN_SINGLE_QUOTES;
		lexer->tokenizer = tokenize_single_quotes;
		return (true);
	}
	return (false);
}

bool	_handle_null_state(t_lex *lexer)
{
	if ((unsigned char)*lexer->ptr == OP_NULL)
	{
		debug_print(_MOD_ ": %s: FOUND NULL TOKEN\n", __FUNCTION__);
		lexer->state = ON_EOF;
		lexer->tokenizer = tokenize_null;
		return (true);
	}
	return (false);
}

bool	_handle_heredoc_state(t_lex *lexer)
{
	if (!lexer->ptr)
		return (false);
	if ((unsigned char)*lexer->ptr == OP_REDIN && (unsigned char)*(lexer->ptr
			+ 1) == OP_REDIN)
	{
		debug_print(_MOD_ ": %s: Transitioning to IN_HEREDOC state\n",
			__FUNCTION__);
		lexer->ptr += 2;
		lexer->state = IN_HEREDOC;
		lexer->tokenizer = tokenize_heredoc;
		return (true);
	}
	return (false);
}

bool	_handle_dollar_state(t_lex *lexer)
{
	if (!lexer->ptr)
		return (false);
	if (!*lexer->ptr)
		return (false);
	if ((unsigned char)*lexer->ptr == OP_ENV
		&& (is_varnamechar((unsigned char)*(lexer->ptr + 1))
			|| is_specialchar((unsigned char)*(lexer->ptr + 1))))
	{
		debug_print(_MOD_ ": %s: Transitioning to IN_DOLLAR state\n",
			__FUNCTION__);
		lexer->state = IN_DOLLAR;
		lexer->tokenizer = tokenize_dollar;
		return (true);
	}
	return (false);
}
