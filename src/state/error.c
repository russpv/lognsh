/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   error.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dayeo <dayeo@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/05 11:53:52 by dayeo             #+#    #+#             */
/*   Updated: 2025/04/05 11:54:40 by dayeo            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "error.h"

void	print_bufflow(void)
{
	write(STDERR_FILENO, SHELL_NAME, ft_strlen(SHELL_NAME));
	write(STDERR_FILENO, ": ", ft_strlen(": "));
	write(STDERR_FILENO, "Input exceeds limits.\n", \
		ft_strlen("Input exceeds limits.\n"));
}

/* Prints errno err for 'dingus' */
void	print_perror(const char *dingus)
{
	write(STDERR_FILENO, SHELL_NAME, ft_strlen(SHELL_NAME));
	write(STDERR_FILENO, ": ", ft_strlen(": "));
	perror(dingus);
}

/* Prints msg for the 'dingus' (no \n added) */
void	print_custom_err(const char *dingus, const char *msg)
{
	write(STDERR_FILENO, SHELL_NAME, ft_strlen(SHELL_NAME));
	write(STDERR_FILENO, ": ", ft_strlen(": "));
	write(STDERR_FILENO, dingus, ft_strlen(dingus));
	write(STDERR_FILENO, ": ", ft_strlen(": "));
	write(STDERR_FILENO, msg, ft_strlen(msg));
}

/* Prints ": Buffer overflow\n"*/
void	print_lex_buffer_overflow(void)
{
	write(STDERR_FILENO, SHELL_NAME, ft_strlen(SHELL_NAME));
	write(STDERR_FILENO, ": Buffer overflow\n", 18);
}

void	print_is_dir(void)
{
	write(STDERR_FILENO, SHELL_NAME, ft_strlen(SHELL_NAME));
	write(STDERR_FILENO, ": Is a directory\n", ft_strlen(": Is a directory\n"));
}
