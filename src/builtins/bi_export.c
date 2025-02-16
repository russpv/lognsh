/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   bi_export.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dayeo <dayeo@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/28 09:17:24 by dayeo             #+#    #+#             */
/*   Updated: 2025/02/16 11:38:42 by dayeo            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "bi_int.h"
#include "../env/env_int.h"
#include "../state/state_int.h"

int	bi_export(t_state *s, char **argv, int argc)
{
	int	error_occurred;
	int	i;

	error_occurred = 0;
	if (argc == 1)
	{
		write(STDERR_FILENO, "minishell: export: \
			no arguments specified\n", 42);
		return (1);
	}
	i = 1;
	while (argv[i])
	{
		if (!process_arg(s, argv[i], &error_occurred))
		{
			i++;
			continue ;
		}
		i++;
	}
	if (error_occurred)
		return (1);
	else
		return (0);
}
