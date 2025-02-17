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
#define ERRMSG_ARGC "too many arguments\n"
#define ERRMSG_ENV "failed to retrieve environment\n"

int	bi_env(t_state *s, char **argv, int argc)
{
	(void)argc;
	if (!argv || !argv[0])
		return (ERR_GENERAL);
	if (argv[1])
	{
		print_custom_err(CMD_NAME, ERRMSG_ARGC);
		return (ERR_GENERAL);
	}
	if (!s || !has_sh_envp(s))
	{
		print_custom_err(CMD_NAME, ERRMSG_ENV);
		return (ERR_GENERAL);
	}
	s_env_print(s);
	return (0);
}
