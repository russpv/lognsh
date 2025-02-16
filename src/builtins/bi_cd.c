/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   bi_cd.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dayeo <dayeo@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/28 09:07:02 by dayeo             #+#    #+#             */
/*   Updated: 2025/01/28 09:14:42 by dayeo            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "bi_int.h"

int	bi_cd(t_state *s, char **argv, int argc)
{
	char	*target_dir;
	char	*old_pwd;
	char	*new_pwd;

	(void)argc;
	(void)s;
	if (argv[2])
	{
		write(STDERR_FILENO,
				SHELL_NAME ": cd: too many arguments\n",
				sizeof(SHELL_NAME ": cd: too manyarguments\n"));
		return (ERR_GENERAL);
	}
	// if no args > do nothing
	if (!argv[1])
		return (0);
	// Assign directory path to target_dir
	target_dir = argv[1];
	// save old pwd before changing
	old_pwd = getenv("PWD");
	// if directory does not exist > error msg
	if (chdir(target_dir) == -1)
	{
		write(STDERR_FILENO, SHELL_NAME ": cd: ", sizeof(SHELL_NAME ": cd: "));
		write(STDERR_FILENO, target_dir, ft_strlen(target_dir));
		write(STDERR_FILENO, ": ", 2);
		write(STDERR_FILENO, strerror(errno), ft_strlen(strerror(errno)));
		write(STDERR_FILENO, "\n", 1);
		return (ERR_CHDIR);
	}
	// new working directory with dynamic allocation
	new_pwd = getcwd(NULL, 0);
	if (!new_pwd)
	{
		write(STDERR_FILENO,
			SHELL_NAME ": cd: error retrieving current directory\n",
			sizeof(SHELL_NAME ": cd: error retrieving current directory\n"));
		return (ERR_GETCWD);
	}
	if (old_pwd)
		setenv("OLDPWD", old_pwd, 1); // RKP: Can't use this func
	setenv("PWD", new_pwd, 1);        // RKP: Can't use this func
	free(new_pwd);                    // must free getcwd(NULL, 0)
	return (0);
}
