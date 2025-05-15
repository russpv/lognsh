/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strncmp.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dayeo <dayeo@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/03 15:52:37 by dayeo             #+#    #+#             */
/*   Updated: 2025/04/03 15:52:37 by dayeo            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "../include/libft.h"
/* STRNCMP
** Returns diff betw values of the first pair of differing bytes
** (as unsigned char) of s1 - s2, comparing no more than n bytes
*/
int	ft_strncmp(const char *str1, const char *str2, size_t n)
{
	unsigned char	*s1;
	unsigned char	*s2;

	s1 = (unsigned char *)str1;
	s2 = (unsigned char *)str2;
	if (n == (size_t) - 1)
	{
		n = ~0;
	}
	if (n == 0 || (*s1 == 0 && *s2 == 0))
		return (0);
	if (*s1 != *s2)
		return (*s1 - *s2);
	while (--n > 0 && *s1 && *s2)
	{
		s1++;
		s2++;
		if (*s1 != *s2)
			return (*s1 - *s2);
	}
	if ((*s1 == 0) ^ (*s2 == 0))
		return (*s1 - *s2);
	return (0);
}
