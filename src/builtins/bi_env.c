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
#include "../state/state.h"

# define CMD_NAME "env"

/* ENV
 * Run a program in a modified environment...
 * Except in this pared down version, 
 * only prints current shell envp content line by line
 */
 int	bi_env(t_state *s, char **argv, int argc)
{
	(void)argc;
	// env should not take any parameters
	if (!argv || !argv[0])
		return (ERR_GENERAL);
	if (argv[1])
	{
		write(2, "env: too many arguments\n", 24);
		return (ERR_GENERAL);
	}
	// check if env list exists(error code matches bash)
	if (!s || !has_sh_envp(s))
	{
		write(2, "env: failed to retrieve environment\n", 35);
		return (ERR_GENERAL);
	}
	write(2, "\033[32m[DEBUG] Printing copied environment...\033[0m\n", 49);
	s_env_print(s);
	write(2, "\033[32m[DEBUG] Finished printing copied environment!\033[0m\n", 56);
	return (0);
}
 
 /* old env
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
} */
