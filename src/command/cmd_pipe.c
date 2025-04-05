/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cmd_pipe.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dayeo <dayeo@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/05 09:26:45 by dayeo             #+#    #+#             */
/*   Updated: 2025/04/05 09:26:46 by dayeo            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "command_int.h"

#define LOGMSG_CPIPE_ANNOUNCE "Cmd: \t### cmd_exec_pipe ###\n"
#define DBGMSG_CPIPE_GOT "Cmd: \t got %d cmds\n"
#define EMSG_CPIPE_PIPE "ERR pipe creation\n"

static int	_setup_pipes(t_mem_mgr *m, t_cmd *c)
{
	int	res;

	res = exec_create_pipes(m, &c->fildes, c->curr_cmdc);
	if (0 != res)
		return (err(EMSG_CPIPE_PIPE), res);
	return (0);
}

/* Closes all pipes in the parent,
 * waits for all pipeline children,
 * then sets and returns exit status of
 * last pipeline command.
 */
static int	_wait_all(t_state *s, t_cmd *c)
{
	int	status;

	exec_close_pipes(c->fildes, c->curr_cmdc);
	if (0 != waitchilds(&status, c->curr_cmdc))
		return (ERR_WAITPID);
	set_exit_status(s, status);
	return (status);
}

static int	_do_pipe_commands(t_state *s, t_list *cmds, t_cmd *c)
{
	t_ast_node	*a;
	int			i;
	int			res;

	i = -1;
	while (cmds && ++i < c->curr_cmdc)
	{
		a = (t_ast_node *)cmds->content;
		res = exec_pipe_fork_redirect_run(s, a, i, cmd_execute_full);
		if (0 != res)
			return (res);
		cmds = cmds->next;
	}
	return (0);
}

/* Executes commands within pipeline node
 * Not guaranteed to have forked, so no cleanup done.
 */
int	cmd_exec_pipe(t_state *s, t_ast_node *pipe)
{
	t_cmd	*cmd;
	int		res;

	log_print(LOGMSG_CPIPE_ANNOUNCE);
	cmd = get_cmd(s);
	if (!cmd)
		return (ERR_ARGS);
	cmd->curr_cmdc = p_get_pipe_cmdc(pipe);
	debug_print(DBGMSG_CPIPE_GOT, cmd->curr_cmdc);
	if (cmd->curr_cmdc < 2)
		return (ERR_INSUFFICIENT_CMDS);
	res = _setup_pipes(get_mem(s), cmd);
	if (0 != res)
		return (res);
	res = _do_pipe_commands(s, p_get_pipe_cmds(pipe), cmd);
	if (0 != res)
		return (res);
	return (_wait_all(s, cmd));
}
