/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strjoin.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rpeavey <rpeavey@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/03 15:52:07 by dayeo             #+#    #+#             */
/*   Updated: 2025/05/15 17:26:28 by rpeavey          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/libft.h"
#include <stdio.h> 
/* STRJOIN
** Returns a new C-string of s1 and s2
** concatenation.
** UNPROTECTED
*/
char	*ft_strjoin(char const *s1, char const *s2)
{
	size_t	len1;
	size_t	len2;
	char	*s3;

	len1 = ft_strlen(s1);
	len2 = ft_strlen(s2);
	s3 = malloc(sizeof(char) * (len1 + len2 + 1));
	if (!s3)
		return (NULL);
	*(s3 + len1 + len2) = 0;
	ft_memcpy(s3, s1, len1);
	ft_memcpy(s3 + len1, s2, len2);
	return (s3);
}

char	*ft_strjoin_mem(t_mem_node *n, t_alloc_fn f, \
char const *s1, char const *s2)
{
	size_t	len1;
	size_t	len2;
	char	*s3;

	if (!s1 || !s2 || !n)
		return (NULL);
	len1 = ft_strlen(s1);
	len2 = ft_strlen(s2);
	s3 = f(n, sizeof(char) * (len1 + len2 + 1));
	if (!s3)
		return (NULL);
	*(s3 + len1 + len2) = 0;
	ft_memcpy(s3, s1, len1);
	ft_memcpy(s3 + len1, s2, len2);
	return (s3);
}
