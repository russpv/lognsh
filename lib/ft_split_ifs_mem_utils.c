/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_split_ifs_mem_utils.c                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dayeo <dayeo@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/03 15:50:32 by dayeo             #+#    #+#             */
/*   Updated: 2025/04/03 15:50:32 by dayeo            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "../include/libft.h"

char	*ft_skip_delims(char const *s, char const *ref)
{
	char	*delims;
	char	*word;

	if (!s || !ref)
		return (NULL);
	word = (char *)s;
	while (*word)
	{
		delims = (char *)ref;
		while (*delims)
		{
			if (*word == *delims)
				break ;
			delims++;
		}
		if (0 == *delims)
			return (word);
		word++;
	}
	return (NULL);
}
