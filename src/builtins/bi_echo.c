/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   bi_echo.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rpeavey <rpeavey@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/28 08:56:22 by dayeo             #+#    #+#             */
/*   Updated: 2025/02/01 01:00:28 by rpeavey          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "bi_int.h"

int	bi_echo(t_state *s, char **argv, int argc)
{
	int	i;
	int	no_newline;

	(void)argc;
	(void)s;
	i = 1;
	no_newline = 0;
	if (!argv)
		return (0);
	while (argv[i] && ft_strncmp(argv[i], "-n", -1) == 0)
	{
		no_newline = 1;
		i++;
	}
	while (argv[i])
	{
		printf("%s", argv[i]);
		if (argv[i + 1])
			printf(" ");
		i++;
	}
	if (!no_newline)
		printf("\n");
	return (0);
}
