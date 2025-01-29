/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   bi_pwd.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dayeo <dayeo@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/28 09:02:05 by dayeo             #+#    #+#             */
/*   Updated: 2025/01/28 09:03:23 by dayeo            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "bi_int.h"

void    builtin_pwd(char **args)
{
    char    *cwd;
    
    if (args[1])
    {
        write(STDERR_FILENO, "minishell: pwd: too many arguments\n", 37);
        g_exit_code = 1;
        return ;
    }
    cwd = getcwd(NULL, 0);
    if (cwd)
    {
        printf("%s\n", cwd);
        free (cwd);
        g_exit_code = 0;
    }
    else
    {
        perror("minishell: pwd");
        g_exit_code = 1;
    }
}