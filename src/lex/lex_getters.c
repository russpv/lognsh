/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lex_getters.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dayeo <dayeo@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/05 11:50:36 by dayeo             #+#    #+#             */
/*   Updated: 2025/04/05 20:50:55 by dayeo            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "lex_int.h"

unsigned int	lex_get_keepd(t_lex *lexer)
{
	if (!lexer)
		return (-1);
	return (lexer->keep_dollar);
}

char	*lex_get_ptr(t_lex *lexer)
{
	if (!lexer)
		return (NULL);
	return (lexer->ptr);
}

const char	*lex_get_raw(t_lex *lexer)
{
	if (!lexer)
		return (NULL);
	return (lexer->raw_string);
}

int	lex_get_lines(t_lex *lexer)
{
	if (!lexer)
		return (-1);
	return (lexer->lines);
}

char	*lex_get_eof(t_lex *lexer)
{
	if (!lexer)
		return (NULL);
	return (lexer->eof_word);
}
