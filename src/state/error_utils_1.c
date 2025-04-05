/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   error_utils_1.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dayeo <dayeo@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/05 20:27:14 by dayeo             #+#    #+#             */
/*   Updated: 2025/04/05 20:27:52 by dayeo            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "error.h"

/* Prints msg for the 'dingus'
 * Note: \n will be printed
 */
void	print_custom_err_err(const char *dingus, const char *gizmo,
		const char *msg)
{
	write(STDERR_FILENO, SHELL_NAME, ft_strlen(SHELL_NAME));
	write(STDERR_FILENO, ": ", ft_strlen(": "));
	write(STDERR_FILENO, dingus, ft_strlen(dingus));
	write(STDERR_FILENO, ": ", ft_strlen(": "));
	write(STDERR_FILENO, "\'", ft_strlen("\'"));
	write(STDERR_FILENO, gizmo, ft_strlen(gizmo));
	write(STDERR_FILENO, "\'", ft_strlen("\'"));
	write(STDERR_FILENO, ": ", ft_strlen(": "));
	write(STDERR_FILENO, msg, ft_strlen(msg));
	write(STDERR_FILENO, "\n", 1);
}

/* Prints ": ambiguous redirect\n"*/
void	print_ambiguous_redirect(const char *orig_fn)
{
	write(STDERR_FILENO, SHELL_NAME, ft_strlen(SHELL_NAME));
	write(STDERR_FILENO, ": ", ft_strlen(": "));
	write(STDERR_FILENO, orig_fn, ft_strlen(orig_fn));
	write(STDERR_FILENO, ": ambiguous redirect\n", 21);
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

#define HDOC_1 "warning: here-document at line "
#define HDOC_2 " delimited by end-of-file (wanted `"

void	print_hdoc_error(const char *line, const char *eof)
{
	ft_putstr_fd(HDOC_1, STDERR_FILENO);
	ft_putstr_fd(line, STDERR_FILENO);
	ft_putstr_fd(HDOC_2, STDERR_FILENO);
	ft_putstr_fd(eof, STDERR_FILENO);
	write(STDERR_FILENO, "\')\n", 3);
}
