/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   bi_env.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dayeo <dayeo@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/28 09:03:39 by dayeo             #+#    #+#             */
/*   Updated: 2025/01/28 09:03:39 by dayeo            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "bi_int.h"

# define CMD_NAME "env"

/* ENV
 * Run a program in a modified environment...
 * Except in this pared down version, 
 * only prints current environ content line by line
 */
int	bi_env(t_state *s, char **argv, int argc)
{
	extern char	**environ;
	int			i;

	(void)s;
	(void)argc;
	if (!argv || !argv[0])
		return (1);
	if (argv[1])
	{
		print_too_many_args(CMD_NAME);
		return (1);
	}
	i = 0;
	while (environ[i])
	{
		printf("%s\n", environ[i]);
		flushbuf();
		i++;
	}
	return (0);
}
