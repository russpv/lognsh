/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   bi_env.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rpeavey <rpeavey@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/28 09:03:39 by dayeo             #+#    #+#             */
/*   Updated: 2025/03/06 17:47:25 by rpeavey          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "bi_int.h"

#define CMD_NAME "env"
#define EMSG_ARGC "No such file or directory.\n"
#define EMSG_ENV "Failed to retrieve environment.\n"

int	bi_env(t_state *s, char **argv, int argc)
{
	(void)argc;
	if (!argv)
		return (ERR_ARGS);
	if (!argv[0])
		return (ERR_ARGS);
	if (argv[1])
	{
		print_custom_err(CMD_NAME, EMSG_ARGC);
		return (127);
	}
	if (!s || !has_sh_envp(s))
	{
		print_custom_err(CMD_NAME, EMSG_ENV);
		return (ERR_GENERAL);
	}
	env_print(get_sh_env_list(s));
	return (0);
}
