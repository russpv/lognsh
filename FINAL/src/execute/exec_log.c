/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_log.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rpeavey <rpeavey@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/10 16:31:43 by rpeavey           #+#    #+#             */
/*   Updated: 2025/04/10 16:31:44 by rpeavey          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "execute_int.h"

#define DBGMSG_ELOGI_ANNOUNCE "Exec: exec_fork_run: got %dth\n"
#define EMSG_ELOGI_FORK "ERR exec_fork_run\n"

/* Child processing for log command
 * Forks, recursively
 * calls top level node executor
 * Note: cannot return exit status of child to parent here
 */
int	exec_fork_run(t_state *s, t_ast_node *node, int i, t_execute_fn executor)
{
	pid_t	pid;
	int		exit_status;

	dprint(DBGMSG_ELOGI_ANNOUNCE, i);
	pid = fork();
	if (pid < 0)
	{
		perror(EMSG_ELOGI_FORK);
		return (ERR_FORK);
	}
	else if (0 == pid)
	{
		sig_reset_handlers();
		exit_status = executor(s, node);
		exit(exit_status);
	}
	return (0);
}
