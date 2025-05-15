/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lex_squote.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rpeavey <rpeavey@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/10 16:32:48 by rpeavey           #+#    #+#             */
/*   Updated: 2025/04/10 16:32:49 by rpeavey          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "lex_int.h"

#define STOP 1

/* Loads a char into buf.
 * Checks for the closing single quote. advances ptr and Returns 1 if found
 * without incrementing ptr.
 * Stops buffer overflow.
 */
static inline int	_load_buf(t_lex *lexer)
{
	dprint(_MOD_ ": %s, ptr:_%c_\n", __FUNCTION__, *lexer->ptr);
	if ((unsigned char)OP_SQUOTE == *lexer->ptr)
	{
		lexer->ptr++;
		return (dprint(_MOD_ ": found closing quote\n"), STOP);
	}
	else
	{
		if (lexer->buf_idx < LEX_BUFSZ - 1)
		{
			if ((unsigned char)OP_NULL == *lexer->ptr)
				return (dprint(_MOD_ ": found null before closing quote\n"),
					STOP);
			lexer->buf[(lexer->buf_idx)++] = *lexer->ptr;
		}
		else
			return (lex_set_err(lexer), pbufflow(NULL), STOP);
	}
	return (0);
}

/* Expected to add only one token to the llist at most
 * Assumes ptr is on the first single quote
 * If '\'' not found, flags incomplete only
 * Ignores missing closing quote.
 * Returns success if token wasn't created (null input)
 * Creates token only if normal delims found (really?)
 */
int	tokenize_single_quotes(t_state *s, t_lex *lexer)
{
	t_tok	*token;

	dprint(_MOD_ YELLOW ": STATE: %s, ptr:_%c_\n" RESET, __FUNCTION__,
		*lexer->ptr);
	while (++lexer->ptr)
		if (STOP == _load_buf(lexer))
			break ;
	if (lexer->lex_err)
		return (ERR_GENERAL);
	if ((unsigned char)OP_NULL == *lexer->ptr)
	{
		dprint(_MOD_ ": WARNING: FOUND NULL\n");
		lexer->input_incomplete = true;
	}
	if (false == is_normal_delim(lexer, 0))
		lexer->is_subtoken = true;
	token = lex_create_token(get_mem(s), lexer, TOK_WORD);
	if (NULL == token)
		return (0);
	if (0 != add_token(get_mem(s), lexer, token))
		return (ERR_GENERAL);
	return (0);
}
