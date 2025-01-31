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

#include "execute_int.h"

/*
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
        //TODO if (execute_system_command(args) < 0)
            perror("minishell: system command not yet done.");
    }
    free(args);     
}*/


/* This wait()'s for all child processes; called by parent 
 * Note: assuming waitpid() behaves irrespective of actual
 * number of children.
 */
void	waitchild(int *status, int childc)
{
	int		i;
	pid_t	child_pid;

	i = -1;
	while (++i < childc)
	{
		child_pid = waitpid(-1, status, 0);
		{
			if (child_pid > 0 && WIFEXITED(*status))
				debug_print("Child %d exited w/ stat:%d\n", child_pid, \
					WEXITSTATUS(*status));
			else if (child_pid > 0 && WIFSIGNALED(*status))
				debug_print("Child %d exited by sig:%d\n", child_pid, \
					WTERMSIG(*status));
		}
	}
	debug_print("All child processes have terminated\n");
}

/* Interpret waitpid() exit status (signals ignored here) */
int	get_exit_status(int status)
{
	int	exit_status;
	int	signal_number;

	exit_status = 0;
	signal_number = 0;
	if (WIFEXITED(status))
		exit_status = WEXITSTATUS(status);
	else if (WIFSIGNALED(status))
		signal_number = WTERMSIG(status);
	(void)signal_number;
	return (exit_status);
}
