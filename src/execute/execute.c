/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dayeo <dayeo@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/24 16:00:49 by dayeo             #+#    #+#             */
/*   Updated: 2025/01/27 13:43:05 by dayeo            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "execute.h"
#include "../parse/parse_int.h"


// Fn to map command names to corresponding built-ins
t_builtin_fn    get_builtin(char *command)
{
    if (ft_strcmp(command, BI_ECHO) == 0)
        return (&builtin_echo);
    if (ft_strcmp(command, BI_PWD) == 0)
        return (&builtin_pwd);
    if (ft_strcmp(command, BI_ENV) == 0)
        return (&builtin_env);
    if (ft_strcmp(command, BI_EXIT) == 0)
        return (&builtin_exit);
    /* WIP
    if (ft_strcmp(command, BI_CD) == 0)
        return (&builtin_cd);
    if (ft_strcmp(command, BI_UNSET) == 0)
        return (&builtin_unset);
    
    if (ft_strcmp(command, BI_EXPORT) == 0)
        return (&builtin_export);
    if (ft_strcmp(command, BI_EXIT) == 0)
        return (&builtin_exit); */
    return (NULL); 
}


// Fn to execute commands
void    execute_command(t_ast_node *node)
{
    char    **args;
    t_builtin_fn    builtin;
    
    if (!node || node->type != AST_NODE_CMD || !node->data.cmd.name)
    {        
        write(STDERR_FILENO, "minishell: Invalid command\n", 28);
        return ;
    }
    args = list_to_array(node->data.cmd.args, node->data.cmd.argc);
    if (!args)
    {
        perror("minishell: malloc");
        return ;
    }
    builtin = get_builtin(node->data.cmd.name);
    if (builtin)
        builtin(args);
    else
    {
        /*if (execute_system_command(args) < 0)*/ /*TODO*/
            perror("minishell: system command not yet done.");
    }
    free(args);     
}


