/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lex_utils_delim.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dayeo <dayeo@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/05 11:51:56 by dayeo             #+#    #+#             */
/*   Updated: 2025/04/05 11:51:57 by dayeo            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "lex_int.h"

static bool	_is_valid_offset(t_lex *l, int offset)
{
	if (!l || !l->ptr)
		return (false);
	if (offset < 0 || l->ptr[offset] == '\0')
		return (false);
	return (true);
}

// If (2-)char sequence matches any NORMALDELIMS
// Looks ahead for any doubles, '&&'
// Returns true if ptr on \0 EOF
bool	is_normal_delim(t_lex *lexer, int offset)
{
	unsigned char	c;
	unsigned char	next;

	if ('\0' == *lexer->ptr)
		return (true);
	if (!_is_valid_offset(lexer, offset))
		return (true);
	c = *lexer->ptr + offset;
	next = is_valid_next(lexer, offset);
	if (c && next)
		debug_print(_MOD_ ": -------- %s:_%c%c_", __FUNCTION__, c, next);
	else if (c)
		debug_print(_MOD_ ": -------- %s:_%c__", __FUNCTION__, c);
	if (ft_strchr(NORMALDELIMS, c))
	{
		if (true == is_logicalop(c, next))
			return (debug_print(" YES\n"), true);
		return (debug_print(" YES\n"), true);
	}
	debug_print(" NO\n");
	return (false);
}
