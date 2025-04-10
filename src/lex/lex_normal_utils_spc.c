/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lex_normal_utils_spc.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rpeavey <rpeavey@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/10 16:32:44 by rpeavey           #+#    #+#             */
/*   Updated: 2025/04/10 16:32:45 by rpeavey          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "lex_int.h"

// Process escape sequences in the lexer
void	process_escape_sequence(t_lex *l)
{
	if ((unsigned char)TK_ESC == *l->ptr)
	{
		l->escape_mode = true;
		l->ptr++;
	}
}

// Sets flags only for special operators like environment variables and globbing
// Advance the lexer pointer past comments #
int	process_special_operators(t_lex *lexer)
{
	unsigned char	prev;

	if ((unsigned char)OP_ENV == *lexer->ptr && ft_varnamelen((lexer->ptr + 1))
		&& false == lexer->escape_mode)
		lexer->do_expansion = true;
	if ((unsigned char)OP_STAR == *lexer->ptr && false == lexer->escape_mode)
		lexer->do_globbing = true;
	if ((unsigned char)OP_HASH == *lexer->ptr && false == lexer->escape_mode)
	{
		prev = is_valid_prev(lexer, 0);
		if (' ' == prev)
		{
			dprint(_MOD_ ": -------- %s got ' #', skipping...\n",
				__FUNCTION__);
			lexer->escape_mode = false;
			while (*lexer->ptr && *lexer->ptr != TK_NEWL
				&& *lexer->ptr != TK_EOF)
				lexer->ptr++;
			return (SKIPREST);
		}
	}
	lexer->escape_mode = false;
	return (0);
}
