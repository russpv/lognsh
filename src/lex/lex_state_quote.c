/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lex_state_quote.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rpeavey <rpeavey@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/10 16:32:54 by rpeavey           #+#    #+#             */
/*   Updated: 2025/04/10 16:32:55 by rpeavey          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "lex_int.h"

static void	_load_single(t_lex *lexer)
{
	dprint(_MOD_ ": %s: Transitioning to IN_SINGLE_QUOTES state\n",
		__FUNCTION__);
	lexer->state = IN_SINGLE_QUOTES;
	lexer->tokenizer = tokenize_single_quotes;
}

static void	_load_dbl(t_lex *lexer)
{
	dprint(_MOD_ ": %s: Transitioning to IN_DOUBLE_QUOTES state\n",
		__FUNCTION__);
	if (IN_DOUBLE_QUOTES != st_int_peek(lexer->stack))
		st_int_push(lexer->stack, IN_DOUBLE_QUOTES);
	lexer->state = IN_DOUBLE_QUOTES;
	lexer->do_wordsplit = false;
	lexer->tokenizer = tokenize_double_quotes;
}

bool	handle_quote_state(t_lex *lexer)
{
	if ((unsigned char)*lexer->ptr == OP_DQUOTE
		|| IN_DOUBLE_QUOTES == st_int_peek(lexer->stack))
	{
		if ((unsigned char)*lexer->ptr == OP_DQUOTE
			&& IN_DOUBLE_QUOTES == st_int_peek(lexer->stack))
		{
			dprint(_MOD_ ": %s: Popping stack and transitioning\n",
				__FUNCTION__);
			st_int_pop(lexer->stack);
			lexer->do_wordsplit = true;
			lexer->ptr++;
			return (false);
		}
		_load_dbl(lexer);
		return (true);
	}
	if ((unsigned char)*lexer->ptr == OP_SQUOTE)
	{
		_load_single(lexer);
		return (true);
	}
	return (false);
}
