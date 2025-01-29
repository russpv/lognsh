/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   bi_exit.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dayeo <dayeo@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/28 09:05:10 by dayeo             #+#    #+#             */
/*   Updated: 2025/01/28 09:05:10 by dayeo            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "bi_int.h"

static int  is_numeric(const char *str)
{
    int     i;

    i = 0;
    if (!str || str[0] == '\0')
        return (0);
    if (str[0] == '-' || str[0] == '+')
        i++;
    while (str[i])
    {
        if (str[i] < '0' || str[i] > '9')
            return (0);
        i++;
    }
    // will not return (1) for lone '-' amd '+' inputs
    return (i > 0);
}

void    builtin_exit(char **args)
{
    int     exit_code;
    
    write(STDOUT_FILENO, "exit\n". 5);

    // if no args: exit with last exit code
    if (!args[1])
        exit(g_exit_code);
    // if first arg is not numeric: exit with 255; shell still exits  
    if (!is_numeric(args[1]))
    {   
        write(STDERR_FILENO, "minishell:exit: numeric argument required\n", 43);
        exit (255);
    }
    // if too many args: update global; do not exit shell; g_exit_code updated
    // only when shell is not exited.  
    if (args[2])
    {
        write(STDERR_FILENO, "minishell:exit: too many arguments\n", 36);
        g_exit_code = 1;
        return ;
    }
    // if valid ard: change ascii to int and mod 256; if code is (-) > add 256
    exit_code = ft_atoi(args[1]) % 256;
    if (exit_code < 0)
        exit_code = exit_code + 256;
    exit(exit_code);
}