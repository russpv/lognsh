/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dayeo <dayeo@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/24 15:43:36 by dayeo             #+#    #+#             */
/*   Updated: 2025/01/25 02:22:39 by dayeo            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "builtins.h"
#include "../../lib/libft.h"
#include <stdio.h>
#include <unistd.h>

int    builtin_echo(char **args)
{
    int     i;
    int     no_newline;

    i = 0;
    no_newline = 0;

    while (args[i] && ft_strncmp(args[i], "-n", -1) == 0)
    {
        no_newline = 1;
        i++;
    }
    while (args[i])
    {
        printf("%s", args[i]);
        if (args[i + 1])
            printf(" ");
        i++;
    }
    if (!no_newline)
        printf("\n");
    return (0);
}

int    builtin_pwd(char **args)
{
    char    *cwd;
    
    if (args[1])
    {
        write(STDERR_FILENO, "minishell: pwd: too many arguments\n", 36);
        return (1);
    }
    cwd = getcwd(NULL, 0);
    if (cwd)
    {
        printf("%s\n", cwd);
        free (cwd);
    }
    else
        perror("minishell: pwd");
    return (0);
}
