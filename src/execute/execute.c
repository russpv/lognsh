/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dayeo <dayeo@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/24 16:00:49 by dayeo             #+#    #+#             */
/*   Updated: 2025/01/25 02:30:02 by dayeo            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../parse/parse_int.h"
#include "execute.h"


// Fn to map command names to corresponding built-ins
t_builtin_fn	get_builtin(char *command)
{
	if (ft_strncmp(command, BI_ECHO, -1) == 0)
		return (&builtin_echo);
	if (ft_strncmp(command, BI_PWD, -1) == 0)
		return (&builtin_pwd);
	/* WIP
	if (ft_strcmp(command, BI_CD) == 0)
		return (&builtin_cd);
	if (ft_strcmp(command, BI_UNSET) == 0)
		return (&builtin_unset);
	if (ft_strcmp(command, BI_ENV) == 0)
		return (&builtin_env);
	if (ft_strcmp(command, BI_EXPORT) == 0)
		return (&builtin_export);
	if (ft_strcmp(command, BI_EXIT) == 0)
		return (&builtin_exit); */
	return (NULL);
}
/*
// Fn to execute commands
void	execute_command(t_ast_node *node)
{
	char			**args;
	t_builtin_fn	builtin;

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
		//if (execute_system_command(args) < 0) TODO
		perror("minishell");
	}
	free(args);
}
*/