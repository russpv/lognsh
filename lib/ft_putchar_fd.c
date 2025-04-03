/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_putchar_fd.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dayeo <dayeo@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/03 15:45:40 by dayeo             #+#    #+#             */
/*   Updated: 2025/04/03 15:45:42 by dayeo            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/libft.h"

/* This write to the passed file descriptor instead of STDOUT */
void	ft_putchar_fd(char c, int fd)
{
	write(fd, &c, 1);
}
