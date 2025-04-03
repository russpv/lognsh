/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_itoa_mem.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dayeo <dayeo@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/03 15:44:30 by dayeo             #+#    #+#             */
/*   Updated: 2025/04/03 15:44:32 by dayeo            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/libft.h"

/* ITOA
** Returns new C-string representing n value
*/

static inline char	*load_str(char *s, long n)
{
	int	sign;

	sign = 0;
	if (n < 0)
	{
		n *= -1;
		sign = 1;
	}
	if (n == 0)
		*s-- = '0';
	while (n != 0)
	{
		*s-- = n % 10 + '0';
		n = n / 10;
	}
	if (sign)
		*s-- = '-';
	s++;
	return (s);
}

char	*ft_itoa_mem(t_mem_node *head, t_alloc_fn f, int num)
{
	char	c[15];
	char	*s;
	char	*dest;

	ft_memset(&c, '\0', 15);
	s = load_str(c + 13, num);
	dest = f(head, sizeof(char) * (ft_strlen(s) + 1));
	if (!dest)
		return (NULL);
	dest[ft_strlen(s)] = 0;
	dest = ft_memcpy(dest, s, ft_strlen(s));
	return (dest);
}
