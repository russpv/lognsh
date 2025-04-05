/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cmd_proc.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dayeo <dayeo@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/05 09:26:51 by dayeo             #+#    #+#             */
/*   Updated: 2025/04/05 09:26:53 by dayeo            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "command_int.h"

#define LOGMSG_CPROC_ANNOUNCE "Cmd: \t### cmd_exec_proc ###\n"
#define DBGMSG_CPROC_GOT "Cmd: \t### cmd_exec_proc got cmd type:%d\n"
#define DBGMSG_CPROC_BADEXEC "Cmd: Command failed with exit status: %d\n"
#define LOGMSG_CPROC_DONE "Cmd: \t### cmd_exec_proc finished execution. ###\n"

/* CMD EXEC PROC
 * Executes proc node.
 * Note: though it stores a t_list, proc can
 * only be one command
 */
int	cmd_exec_proc(t_state *s, t_ast_node *a)
{
	const int		cmdc = p_get_proc_cmdc(a);
	const t_list	*cmd = p_get_proc_cmds(a);
	t_ast_node		*node;
	int				i;
	int				exit_status;

	node = NULL;
	log_print(LOGMSG_CPROC_ANNOUNCE);
	i = -1;
	if (0 == cmdc || NULL == cmd)
		return (0);
	while (++i < cmdc)
	{
		node = cmd->content;
		debug_print(DBGMSG_CPROC_GOT, p_get_type(node));
		exit_status = exec_fork_wait(s, node, cmd_execute_full);
		if (exit_status != 0)
			debug_print(DBGMSG_CPROC_BADEXEC, exit_status);
		cmd = cmd->next;
	}
	log_print(LOGMSG_CPROC_DONE);
	return (exit_status);
}
