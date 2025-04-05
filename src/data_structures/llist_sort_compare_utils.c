/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   llist_sort_compare_utils.c                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dayeo <dayeo@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/05 21:59:16 by dayeo             #+#    #+#             */
/*   Updated: 2025/04/05 22:03:45 by dayeo            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "llist_int.h"

void	_get_root(char *buf, void *s)
{
	char	*str;
	size_t	buf_idx;

	str = (char *)s;
	buf_idx = 0;
	while (*str && buf_idx < MAX_BUF)
	{
		if (ft_isalnum(*str))
			buf[buf_idx++] = *str;
		str++;
	}
}
