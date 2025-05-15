/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strlen.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dayeo <dayeo@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/03 15:52:22 by dayeo             #+#    #+#             */
/*   Updated: 2025/04/03 15:52:23 by dayeo            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "../include/libft.h"
/* STRLEN
** This returns length of the string pointed to by 'c'
** NOT including '\0' (e.g. 'size')
** Will NOT SEGV if c is NULL (UNlike strlen())
*/
size_t	ft_strlen(const char *c)
{
	size_t	i;

	i = 0;
	if (NULL == c)
		return (0);
	while (c[i])
		i++;
	return (i);
}
