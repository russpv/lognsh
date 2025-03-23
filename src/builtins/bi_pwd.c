/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   bi_pwd.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rpeavey <rpeavey@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/28 09:02:05 by dayeo             #+#    #+#             */
/*   Updated: 2025/03/17 18:55:16 by rpeavey          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "bi_int.h"

#define CMD_NAME "pwd"
#define EMSG_INVLD "invalid state or arguments\n"
#define EMSG_PWDNOTSET "PWD cannot get current directory environment.\n"

int	bi_pwd(t_state *s, char **argv, int argc)
{
	char	*pwd;

	(void)argc;
	if (!s)
	{
		print_custom_err(CMD_NAME, EMSG_INVLD);
		return (ERR_GENERAL);
	}
	if (argv[1])
	{
		if (argv[1][0] == '-')
			return (print_custom_err_err(CMD_NAME, argv[1], EMSG_OPT),
				ERR_GENERAL);
	}
	pwd = get_pwd(s);
	if (!pwd)
	{
		print_custom_err(CMD_NAME, EMSG_PWDNOTSET);
		return (ERR_GENERAL);
	}
	write(STDOUT_FILENO, pwd, ft_strlen(pwd));
	write(STDOUT_FILENO, "\n", 1);
	return (0);
}
