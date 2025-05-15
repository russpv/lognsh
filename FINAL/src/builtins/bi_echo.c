/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   bi_echo.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rpeavey <rpeavey@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/10 16:26:22 by rpeavey           #+#    #+#             */
/*   Updated: 2025/04/10 16:26:23 by rpeavey          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "bi_int.h"

/* ECHO
 * derived from code echo.c in Bash.
 * Option -n defeats newline print
 */
static void	_print_args(char **argv, int *first_arg, int argc, int i)
{
	while (i < argc)
	{
		if (!*first_arg)
			write(STDOUT_FILENO, " ", 1);
		write(STDOUT_FILENO, argv[i], ft_strnlen(argv[i], INPUT_BUF_SZ));
		*first_arg = 0;
		i++;
	}
}

int	bi_echo(t_state *s, char **argv, int argc)
{
	int	i;
	int	j;
	int	first_arg;
	int	suppress_newline;

	(void)s;
	i = 1;
	suppress_newline = 0;
	first_arg = 1;
	while (i < argc && argv[i][0] == '-')
	{
		j = 1;
		while (argv[i] && argv[i][j] == 'n')
			j++;
		if (argv[i][j] == '\0')
			suppress_newline = 1;
		else
			break ;
		i++;
	}
	_print_args(argv, &first_arg, argc, i);
	if (!suppress_newline && (argc == 1 || !first_arg))
		write(STDOUT_FILENO, "\n", 1);
	return (0);
}
