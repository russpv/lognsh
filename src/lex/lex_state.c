/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lex_state.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rpeavey <rpeavey@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/10 16:32:57 by rpeavey           #+#    #+#             */
/*   Updated: 2025/04/12 17:12:09 by rpeavey          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "lex_int.h"

/* The State Machine
 * General states:
 * START: Beginning of input or start of a new token.
 * IN_WORD: Collecting characters in a word token.
 * IN_OPERATOR: Collecting characters in an operator token.
 * IN_QUOTE: Processing quoted text.
 * IN_SUBSTITUTION: Processing parameter, command, or arithmetic substitution.
 * IN_COMMENT: Discarding characters in a comment.
 * IN_HEREDOC:
 * END: End of input.
 *
 * Use token buffer and token list
 *
 * Traverse input ONCE
 *
 * Handle these rules
 * Hash - Ignore everything till newline
 * Whitespace - discard unquoted space and finalize current token
 * Start char - start a new token
 * Quotes - track ' " or \ and collect until closing symbol found
 * Operators - look ahead one char for double char ops
 * Null term - finalize current token
 * (distinguish reserved words)
 * (remove escape chars)
 *
 * Tokens
 * TOKEN: WORD, ASSIGNMENT_WORD, NAME, (reserved word)
 * OPERATOR: (below)
 * NEWLINE
 * IO_NUMBER e.g. &2>1
 *
 * AND_IF
 * OR_IF
 * DLESS
 * DGREAT
 */

// Stops tokenizer loop from running
// ON_EOF can be set only after NULL token is added
static inline bool	_handle_eof_state(t_lex *lexer)
{
	if (ON_EOF == lexer->state)
	{
		dprint(_MOD_ ": %s: Transitioning to DONE state\n", __FUNCTION__);
		lexer->state = DONE;
		return (true);
	}
	return (false);
}

static inline bool	_handle_null_state(t_lex *lexer)
{
	if ((unsigned char)*lexer->ptr == OP_NULL)
	{
		dprint(_MOD_ ": %s: FOUND NULL TOKEN\n", __FUNCTION__);
		lexer->state = ON_EOF;
		lexer->tokenizer = tokenize_null;
		return (true);
	}
	return (false);
}

static inline bool	_handle_heredoc_state(t_lex *lexer)
{
	if (!lexer->ptr)
		return (false);
	if ((unsigned char)*lexer->ptr == OP_REDIN && (unsigned char)*(lexer->ptr
			+ 1) == OP_REDIN)
	{
		dprint(_MOD_ ": %s: Transitioning to IN_HEREDOC state\n",
			__FUNCTION__);
		lexer->ptr += 2;
		lexer->state = IN_HEREDOC;
		lexer->tokenizer = tokenize_heredoc;
		return (true);
	}
	return (false);
}

static inline bool	_handle_dollar_state(t_lex *lexer)
{
	if (!lexer->ptr)
		return (false);
	if (!*lexer->ptr)
		return (false);
	if ((unsigned char)*lexer->ptr == OP_ENV
		&& (is_varnamechar((unsigned char)*(lexer->ptr + 1))
			|| is_specialchar((unsigned char)*(lexer->ptr + 1))))
	{
		dprint(_MOD_ ": %s: Transitioning to IN_DOLLAR state\n",
			__FUNCTION__);
		lexer->state = IN_DOLLAR;
		lexer->tokenizer = tokenize_dollar;
		return (true);
	}
	return (false);
}

/* Changes state based on current ptr position
 * All state changes done here.
 * Called by tokenize() loop.
 * Order matters.
 */
int	do_state_transition(t_lex *lexer)
{
	dprint(_MOD_ ": %s _%c_\n", __FUNCTION__, *lexer->ptr);
	if (!lexer->ptr)
		return (0);
	if (_handle_heredoc_state(lexer))
		return (0);
	if (_handle_eof_state(lexer))
		return (0);
	if (_handle_null_state(lexer))
		return (0);
	if (_handle_dollar_state(lexer))
		return (0);
	if (handle_quote_state(lexer))
		return (0);
	lexer->state = NORMAL;
	lexer->do_wordsplit = true;
	lexer->tokenizer = tokenize_normal;
	return (0);
}
