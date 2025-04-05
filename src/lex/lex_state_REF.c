/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lex_state_REF.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dayeo <dayeo@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/05 11:51:41 by dayeo             #+#    #+#             */
/*   Updated: 2025/04/05 20:55:05 by dayeo            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "lex_int.h"

/* Changes state based on current ptr position
 * All state changes done here.
 * Called by tokenize() loop.
 * Order matters.
 */
int	do_state_transition(t_lex *lexer)
{
	debug_print(_MOD_ ": %s _%c_\n", __FUNCTION__, *lexer->ptr);
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
	if (_handle_quote_state(lexer))
		return (0);
	lexer->state = NORMAL;
	lexer->tokenizer = tokenize_normal;
	return (0);
}
