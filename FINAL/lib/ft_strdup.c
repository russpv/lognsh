/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strdup.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dayeo <dayeo@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/03 15:51:47 by dayeo             #+#    #+#             */
/*   Updated: 2025/04/03 15:51:47 by dayeo            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "../include/libft.h"
/* STRDUP
** This returns pointer to new string
** malloc handles errno
*/
char	*ft_strdup(const char *s)
{
	char	*newstr;
	size_t	len;

	len = ft_strlen(s);
	newstr = (char *)malloc(sizeof(char) * len + 1);
	if (!newstr)
		return (NULL);
	newstr[len] = 0;
	return (ft_memcpy(newstr, s, len));
}
