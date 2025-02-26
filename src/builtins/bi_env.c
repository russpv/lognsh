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

#define CMD_NAME "env"
#define ERRMSG_ARGC "No such file or directory.\n"
#define ERRMSG_ENV "failed to retrieve environment\n"

int	bi_env(t_state *s, char **argv, int argc)
{
	(void)argc;
	if (!argv || !argv[0])
		return (ERR_GENERAL);
	if (argv[1])
	{
		print_custom_err(CMD_NAME, ERRMSG_ARGC);
		return (127);
	}
	if (!s || !has_sh_envp(s))
	{
		print_custom_err(CMD_NAME, ERRMSG_ENV);
		return (ERR_GENERAL);
	}
	env_print(get_sh_env_list(s));
	return (0);
}
