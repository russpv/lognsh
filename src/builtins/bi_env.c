/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   bi_env.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dayeo <dayeo@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/28 09:03:39 by dayeo             #+#    #+#             */
/*   Updated: 2025/01/28 09:03:39 by dayeo            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "bi_int.h"

void    builtin_env(char **args)
{
    extern char **environ;
    int i;

    if (args[1])
    {
        write(STRERR_FILENO, "minishell: env: too many arguments\n", 35);
        g_exit_code = 1;
        return ;
    }
    i = 0;
    while (environ[i])
    {
        printf("%s\n", environ[i]);
        i++;
    }
    g_exit_code = 0;
}