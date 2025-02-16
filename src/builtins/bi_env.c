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

#define CMD_NAME "env"

int	bi_env(t_state *s, char **argv, int argc)
{
	(void)argc;
	if (!argv || !argv[0])
		return (ERR_GENERAL);
	if (argv[1])
	{
		write(2, "env: too many arguments\n", 24);
		return (ERR_GENERAL);
	}
	if (!s || !has_sh_envp(s))
	{
		write(2, "env: failed to retrieve environment\n", 35);
		return (ERR_GENERAL);
	}
	s_env_print(s);
	return (0);
}
