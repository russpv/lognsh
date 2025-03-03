/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   bi_pwd.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dayeo <dayeo@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/28 09:02:05 by dayeo             #+#    #+#             */
/*   Updated: 2025/02/16 15:15:55 by dayeo            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "bi_int.h"
#define ERRMSG_INVLD "invalid state or arguments\n"
#define ERRMSG_PWDNOTSET "PWD cannot get current directory environment.\n"

int	bi_pwd(t_state *s, char **args, int argc)
{
	char	*pwd;

	(void)args;
	(void)argc;
	if (!s)
	{
		print_custom_err("pwd", ERRMSG_INVLD);
		return (1);
	}
	pwd = getcwd(NULL, 0);
	if (!pwd)
	{
		print_custom_err("pwd", ERRMSG_PWDNOTSET);
		return (1);
	}
	write(STDOUT_FILENO, pwd, ft_strlen(pwd));
	write(STDOUT_FILENO, "\n", 1);
	free(pwd);
	return (0);
}
