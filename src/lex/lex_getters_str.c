/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lex_getters_str.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rpeavey <rpeavey@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/10 16:32:27 by rpeavey           #+#    #+#             */
/*   Updated: 2025/04/10 16:32:28 by rpeavey          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "lex_int.h"

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

char	*lex_get_eof(t_lex *lexer)
{
	if (!lexer)
		return (NULL);
	return (lexer->eof_word);
}
