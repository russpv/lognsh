/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strcmplow.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dayeo <dayeo@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/03 15:51:27 by dayeo             #+#    #+#             */
/*   Updated: 2025/04/03 15:51:29 by dayeo            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "../include/libft.h"
#include <stdio.h>

int	ft_strcmp_low(const char *s1, const char *s2)
{
	size_t				i;
	int					res;

	i = 0;
	while (s1[i] || s2[i])
	{
		const unsigned char	l = ft_tolower((unsigned char)s1[i]);
		const unsigned char	r = ft_tolower((unsigned char)s2[i]);
		if (l != r)
		{
			res = l - r;
			return (res);
		}
		i++;
	}
	return (0);
}
