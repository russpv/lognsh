/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   llist_sort_compare_utils.c                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rpeavey <rpeavey@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/10 16:30:30 by rpeavey           #+#    #+#             */
/*   Updated: 2025/04/10 16:30:31 by rpeavey          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "llist_int.h"

void	get_root(char *buf, void *s)
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
