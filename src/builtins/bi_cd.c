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
	char    *target_dir;
	char	*old_pwd;
	char	*new_pwd;

	(void)argc;
	(void)s;
	// if too many arguments error message; 
	if (argv[2])
	{
		write(STDERR_FILENO, "minishell: cd: too many \
			arguments\n", 36);
        	return (1);
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
	 	write(STDERR_FILENO, "minishell: cd: ", 15);
	 	write(STDERR_FILENO, target_dir, ft_strlen(target_dir));
	 	write(STDERR_FILENO, ": ", 2);
	 	write(STDERR_FILENO, strerror(errno), ft_strlen(strerror(errno)));
	 	write(STDERR_FILENO, "\n", 1);
		return (1);
	}
    	// new working directory with dynamic allocation
    	new_pwd = getcwd(NULL, 0);
    	if (!new_pwd)
    	{
    		write(STDERR_FILENO, "minish: cd: error retrieving current directory\n", 48);
    		return (1);
    	}
    	
    	if (old_pwd)
    		setenv("OLDPWD", old_pwd, 1);
    	setenv("PWD", new_pwd, 1);
    	free(new_pwd); // must free getcwd(NULL, 0)
    	
    	return (0);
    	
 }
