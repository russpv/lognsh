/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lex_utils_2.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dayeo <dayeo@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/06 01:14:25 by dayeo             #+#    #+#             */
/*   Updated: 2025/04/06 01:14:34 by dayeo            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "lex_int.h"

bool	is_varnamechar(unsigned char c)
{
	debug_print(_MOD_ ": %s: (%c)\n", __FUNCTION__, c);
	return (ft_isalnum((int)c) || '_' == c);
}

// Checks for any special substition char
bool	is_specialchar(unsigned char c)
{
	debug_print(_MOD_ ": %s: (%c)\n", __FUNCTION__, c);
	return ('?' == c);
}
