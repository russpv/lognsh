/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cmd_log.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dayeo <dayeo@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/05 09:26:38 by dayeo             #+#    #+#             */
/*   Updated: 2025/04/05 09:26:39 by dayeo            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "command_int.h"

#define HALT_EXEC -1
#define DBGMSG_CLOGI_ANNOUNCE "Cmd: \t### cmd_exec_log ###\n"
#define DBGMSG_CLOGI_DONE "Cmd: \t### cmd_exec_log: got exit, returning\n"
#define DBGMSG_CLOGI_EXECERR "Cmd: \t### cmd_exec_log: got negative exit,\
	returning\n"
#define DBGMSG_CLOGI_GOT "Cmd: \t got %d cmds\n"
#define DBGMSG_CLOGI_GOT2 "Cmd: \t### _do_log_commands got cmd:%s\n"
#define DBGMSG_CLOGI_GOT3 "Cmd: \t### _do_log_commands got exit:%d\n"
#define DBGMSG_CLOGI_GOT4 "Cmd: \t### _do_log_commands got op:%s\n"
#define LOGMSG_CLOGI_STOP "Cmd: Operator evalution stopping further execution\n"

static int	_do_more_ops(const t_list *ops, int exit_status)
{
	debug_print(DBGMSG_CLOGI_GOT3, exit_status);
	if (!ops)
		return (HALT_EXEC);
	debug_print(DBGMSG_CLOGI_GOT4, ops->content);
	if ((0 != exit_status && 0 == ft_strcmp(OP_ANDIF, ops->content))
		|| (0 == exit_status && 0 == ft_strcmp(OP_ORIF, ops->content)))
	{
		log_print(LOGMSG_CLOGI_STOP);
		return (HALT_EXEC);
	}
	return (0);
}

/* Evaluates logical operators for conditional
 * execution and returns the last exit status
 */
static int	_do_log_commands(t_state *s, t_list *cmds, t_cmd *c)
{
	t_ast_node		*a;
	const t_list	*ops = (const t_list *)p_get_log_ops(c->curr_node);
	int				exit_status;
	int				i;

	i = -1;
	while (cmds && ++i < c->curr_cmdc)
	{
		a = (t_ast_node *)cmds->content;
		debug_print(DBGMSG_CLOGI_GOT2, p_get_cmd(a));
		if (0 != exec_fork_run(s, a, i, cmd_execute_full))
			return (-1);
		if (0 != waitchilds(&exit_status, 1))
			return (-1);
		if (HALT_EXEC == _do_more_ops(ops, exit_status))
			break ;
		cmds = cmds->next;
		ops = ops->next;
	}
	c->curr_cmdc = i + 1;
	return (exit_status);
}

int	cmd_exec_log(t_state *s, t_ast_node *node)
{
	t_cmd	*cmd;
	int		exit_status;

	log_print(DBGMSG_CLOGI_ANNOUNCE);
	cmd = get_cmd(s);
	if (!cmd)
		return (-1);
	cmd->curr_cmdc = p_get_log_cmdc(node);
	cmd->curr_node = node;
	debug_print(DBGMSG_CLOGI_GOT, cmd->curr_cmdc);
	if (cmd->curr_cmdc < 2)
		return (-1);
	exit_status = _do_log_commands(s, p_get_log_cmds(node), cmd);
	if (exit_status < 0)
	{
		debug_print(DBGMSG_CLOGI_EXECERR);
		return (-1);
	}
	log_print(DBGMSG_CLOGI_DONE);
	return (exit_status);
}
