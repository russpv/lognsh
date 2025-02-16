/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   bi_echo.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dayeo <dayeo@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/28 08:56:22 by dayeo             #+#    #+#             */
/*   Updated: 2025/02/16 17:20:16 by dayeo            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "bi_int.h"

/* ECHO
 * derived from code echo.c in Bash.
 * Option -n.* defeats newline print
 */

int	bi_echo(t_state *s, char **argv, int argc)
{
	int		i;
	int		suppress_newline;

	(void)s;
	i = 1;
	suppress_newline = 0;
	if (argc > 1 && ft_strcmp(argv[1], "-n") == 0)
	{
		suppress_newline = 1;
		i++;
	}
	while (i < argc)
	{
		write(STDOUT_FILENO, argv[i], ft_strlen(argv[i]));
		if (i < argc - 1)
			write(STDOUT_FILENO, " ", 1);
		i++;
	}
	if (!suppress_newline)
		write(STDOUT_FILENO, "\n", 1);
	return (0);
}
