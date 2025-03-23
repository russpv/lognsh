/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   bi_env.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rpeavey <rpeavey@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/28 09:03:39 by dayeo             #+#    #+#             */
/*   Updated: 2025/03/17 18:53:50 by rpeavey          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "bi_int.h"

#define CMD_NAME "env"
#define EMSG_ENV "Failed to retrieve environment.\n"

// Prints the shell's copy of env
int	bi_env(t_state *s, char **argv, int argc)
{
	(void)argc;
	if (!argv)
		return (ERR_ARGS);
	if (!argv[0])
		return (ERR_ARGS);
	if (argv[1])
	{
		if (argv[1][0] == '-')
			print_custom_err_err(CMD_NAME, argv[1], EMSG_OPT);
		else
			print_custom_err_err(CMD_NAME, argv[1], EMSG_ARGC);
		return (127);
	}
	if (!s || !has_sh_envp(s))
	{
		print_custom_err(CMD_NAME, EMSG_ENV);
		return (ERR_GENERAL);
	}
	env_print(get_env_list(s));
	return (0);
}
