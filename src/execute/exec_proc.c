/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_proc.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dayeo <dayeo@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/05 11:48:57 by dayeo             #+#    #+#             */
/*   Updated: 2025/04/05 11:48:59 by dayeo            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "execute_int.h"

/* Forks child and runs executor for
 * higher-level AST nodes.
 */
int	exec_fork_wait(t_state *s, t_ast_node *node, t_execute_fn executor)
{
	pid_t	pid;
	int		status;
	int		exit_status;

	exit_status = 0;
	if (SIGINT == g_last_signal)
		return (SIGINT_BEFORE_FORK);
	pid = fork();
	if (0 > pid)
	{
		err(EMSG_FORK);
		return (ERR_FORK);
	}
	else if (0 == pid)
	{
		sig_reset_handlers();
		exit_status = executor(s, node);
		destroy_state(&s);
		exit(exit_status);
	}
	waitchilds(&status, 1);
	set_exit_status(s, status);
	sig_set_handlers(INT_DFL);
	return (exec_get_exit_status(status));
}
