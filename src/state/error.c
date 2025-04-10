/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   error.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rpeavey <rpeavey@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/10 16:34:42 by rpeavey           #+#    #+#             */
/*   Updated: 2025/04/10 16:34:43 by rpeavey          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "error.h"

void	pbufflow(const char *msg)
{
	const int	fd = STDERR_FILENO;

	write(fd, SHELL_NAME, ft_strlen(SHELL_NAME));
	write(fd, ": ", ft_strlen(": "));
	if (NULL == msg)
		ft_putstr_fd("Input exceeds limits.\n", fd);
	else
	{
		ft_putstr_fd("Input exceeds limits:\n", fd);
		ft_putstr_fd(msg, fd);
		write(fd, "\n", 1);
	}
}

/* Prints ": Buffer overflow\n"*/
void	print_lex_buffer_overflow(void)
{
	write(STDERR_FILENO, SHELL_NAME, ft_strlen(SHELL_NAME));
	write(STDERR_FILENO, ": Buffer overflow\n", 18);
}

/* Prints "cmd: command not found\n"*/
void	print_command_not_found(const char *cmd)
{
	write(STDERR_FILENO, SHELL_NAME, ft_strlen(SHELL_NAME));
	write(STDERR_FILENO, ": ", ft_strlen(": "));
	if (cmd)
		write(STDERR_FILENO, cmd, ft_strlen(cmd));
	else
		write(STDERR_FILENO, "(null)", ft_strlen("(null)"));
	write(STDERR_FILENO, ": command not found\n", 20);
}

int	get_wincols(void)
{
	struct winsize	ws;

	if (-1 == ioctl(STDOUT_FILENO, TIOCGWINSZ, &ws))
	{
		perror("ioctl");
		return (ERR_GENERAL);
	}
	return (ws.ws_row);
}

void	print_inv_cmd(void)
{
	write(STDERR_FILENO, SHELL_NAME, ft_strlen(SHELL_NAME));
	ft_putstr_fd(": Invalid command.\n", STDERR_FILENO);
}
