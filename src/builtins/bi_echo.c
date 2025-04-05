/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   bi_echo.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dayeo <dayeo@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/28 08:56:22 by dayeo             #+#    #+#             */
/*   Updated: 2025/04/03 00:45:31 by dayeo            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "bi_int.h"

/* ECHO
 * derived from code echo.c in Bash.
 * Option -n defeats newline print
 */
static void	print_arg(char *arg, int *first_arg)
{
	if (!*first_arg)
		write(STDOUT_FILENO, " ", 1);
	write(STDOUT_FILENO, arg, ft_strlen(arg));
	*first_arg = 0;
}

int	bi_echo(t_state *s, char **argv, int argc)
{
	int	i;
	int	first_arg;
	int	suppress_newline;

	(void)s;
	i = 1;
	suppress_newline = 0;
	first_arg = 1;
	while (i < argc && argv[i][0] == '-' && argv[i][1] == 'n' && argv[i][2] == ' ')
	{
		suppress_newline = 1;
		i++;
	}
	while (i < argc)
	{
		print_arg(argv[i], &first_arg);
		i++;
	}
	if (!suppress_newline && (argc == 1 || !first_arg))
		write(STDOUT_FILENO, "\n", 1);
	return (0);
}
