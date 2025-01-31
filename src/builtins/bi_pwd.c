/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   bi_pwd.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rpeavey <rpeavey@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/28 09:02:05 by dayeo             #+#    #+#             */
/*   Updated: 2025/02/01 01:00:55 by rpeavey          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "bi_int.h"

int    bi_pwd(t_state *s, char **args)
{
    char    *cwd;

    (void)s;
    if (args[1])
    {
        write(STDERR_FILENO, "minishell: pwd: too many arguments\n", sizeof("minishell: pwd: too many arguments\n"));
        return (1);
    }
    cwd = getcwd(NULL, 0);
    if (cwd)
    {
        printf("%s\n", cwd);
        free (cwd);
        return (0);
    }
    else
    {
        perror("minishell: pwd");
        return (1);
    }
}