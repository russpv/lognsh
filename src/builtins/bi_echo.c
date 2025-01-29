/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   bi_echo.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dayeo <dayeo@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/28 08:56:22 by dayeo             #+#    #+#             */
/*   Updated: 2025/01/28 09:03:58 by dayeo            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "bi_int.h"

int    bi_echo(char **args)
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
