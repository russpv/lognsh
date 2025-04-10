/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lex_utils_delim_bools.c                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rpeavey <rpeavey@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/10 16:32:59 by rpeavey           #+#    #+#             */
/*   Updated: 2025/04/10 16:33:00 by rpeavey          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "lex_int.h"

// Checks for '&&'
bool	is_logicalop(unsigned char c, unsigned char next)
{
	if ('&' == c)
	{
		if (next)
			if ('&' == next)
				return (true);
	}
	return (false);
}
