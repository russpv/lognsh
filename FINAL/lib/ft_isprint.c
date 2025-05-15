/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_isprint.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dayeo <dayeo@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/03 15:28:55 by dayeo             #+#    #+#             */
/*   Updated: 2025/04/03 15:28:56 by dayeo            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/libft.h"

/* ISPRINT
** returns non-zero if printable char 
*/

t_bool	ft_isprint(int c)
{
	if (c >= ' ' && c <= '~')
		return (TRUE);
	else
		return (FALSE);
}
