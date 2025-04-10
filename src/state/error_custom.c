/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   error_custom.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rpeavey <rpeavey@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/10 16:34:37 by rpeavey           #+#    #+#             */
/*   Updated: 2025/04/10 16:34:38 by rpeavey          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "error.h"

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

/* Prints msg for the 'dingus'
 * Note: \n will be printed
 */
void	print_custom_err_err(const char *dingus, const char *gizmo,
		const char *msg)
{
	if (isatty(STDERR_FILENO))
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
}
