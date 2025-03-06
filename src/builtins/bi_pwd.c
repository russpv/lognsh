/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   bi_pwd.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rpeavey <rpeavey@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/28 09:02:05 by dayeo             #+#    #+#             */
/*   Updated: 2025/03/06 17:10:27 by rpeavey          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "bi_int.h"
#define EMSG_INVLD "invalid state or arguments\n"
#define EMSG_PWDNOTSET "PWD cannot get current directory environment.\n"

int	bi_pwd(t_state *s, char **args, int argc)
{
	char	*pwd;

	(void)args;
	(void)argc;
	if (!s)
	{
		print_custom_err("pwd", EMSG_INVLD);
		return (1);
	}
	pwd = getcwd(NULL, 0);
	if (!pwd)
	{
		print_custom_err("pwd", EMSG_PWDNOTSET);
		return (1);
	}
	write(STDOUT_FILENO, pwd, ft_strlen(pwd));
	write(STDOUT_FILENO, "\n", 1);
	free(pwd);
	return (0);
}
