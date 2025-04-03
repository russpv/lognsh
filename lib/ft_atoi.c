/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_atoi.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dayeo <dayeo@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/03 15:26:17 by dayeo             #+#    #+#             */
/*   Updated: 2025/04/03 15:26:19 by dayeo            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/libft.h"

/* ATOI
** Returns int value of byte string.
** Skips any isspace(3)
** No error checking.
*/
int	ft_atoi(const char *s)
{
	int	num;
	int	flag;

	flag = 1;
	num = 0;
	while (ft_isspace(*s))
		s++;
	if (*s == '-' || *s == '+')
	{
		if (*s == '-')
			flag *= -1;
		s++;
	}
	while (ft_isdigit(*s))
	{
		num *= 10;
		num += *s - '0';
		s++;
	}
	return (flag * num);
}
