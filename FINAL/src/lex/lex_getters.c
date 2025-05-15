/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lex_getters.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rpeavey <rpeavey@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/10 16:32:28 by rpeavey           #+#    #+#             */
/*   Updated: 2025/04/10 16:32:29 by rpeavey          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "lex_int.h"

unsigned int	lex_get_keepd(t_lex *lexer)
{
	if (!lexer)
		return (-1);
	return (lexer->keep_dollar);
}

bool	lex_get_incomplete(t_lex *lexer)
{
	if (!lexer)
		return (false);
	return (lexer->input_incomplete);
}

int	lex_get_lines(t_lex *lexer)
{
	if (!lexer)
		return (-1);
	return (lexer->lines);
}

t_list	*lex_get_tokens(t_lex *lexer)
{
	if (!lexer)
		return (NULL);
	return (lexer->token_list);
}
