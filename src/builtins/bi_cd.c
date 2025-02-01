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

/*
void    builtin_cd(char **args)
{
    char    *target_dir;

    // if too many arguments > error message; g_exit = 1
    if (args[2])
    {
        write(STDERR_FILENO, "minishell: cd: too many arguments\n", 36);
        g_exit_code = 1;
        return ;
    }
    // if no args > go to HOME
    if (!args[1])
    {
        target_dir = getenv("HOME");
        if (!target_dir)
        {
            write(STDERR_FILENO, "minishell: cd: HOME not set\n", 28);
            g_exit_code = 1;
            return ;
        }
    }
    // assigns directory path to target_dir
    else
        target_dir = args[1];
    // if directory does not exist > error msg
    if (chdir(target_dir) == -1)
    {
        write(STDERR_FILENO, "minishell: cd: ", 15);
        write(STDERR_FILENO, target_dir, ft_strlen(target_dir));
        write(STDERR_FILENO, ": ", 2);
        write(STDERR_FILENO, strerror(errno), ft_strlen(strerror(errno)));
        write(STDERR_FILENO, "\n", 1);
        g_exit_code = 1;
        return ;
    }
    // successful change dir > update prompt
    g_exit_code = 0;
}*/