/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strndup.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dayeo <marvin@42.fr>                       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/20 00:09:36 by dayeo             #+#    #+#             */
/*   Updated: 2023/09/20 03:00:48 by dayeo            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "../include/libft.h"

char	*ft_strndup(const char *s, size_t size)
{
	char	*dup;
	size_t	i;

	if (!s)
		return (NULL);
	dup = malloc(size + 1);
	if (!dup)
		return (NULL);
	i = 0;
	while (i < size && s[i])
	{
		dup[i] = s[i];
		i++;
	}
	dup[i] = '\0';
	return (dup);
}
