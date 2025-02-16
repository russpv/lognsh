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

int    bi_pwd(t_state *s, char **args, int argc)
{
    char    *cwd;

    (void)s;
    (void)argc;
    if (args[1])
    {
        write(STDERR_FILENO, SHELL_NAME": pwd: too many arguments\n", sizeof(SHELL_NAME": pwd: too many arguments\n"));
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
        perror(SHELL_NAME": pwd");
        return (ERR_GETCWD);
    }
}
