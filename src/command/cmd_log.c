#include "command_int.h"

/* Evaluates logical operators for conditional
 * execution and returns the last exit status
 */
static int	_do_log_commands(t_state *s, t_list *cmds, t_cmd *c)
{
	t_ast_node *a;
	const t_list *ops = (const t_list *)p_get_log_ops(c->curr_node);
	int exit_status;
	int i;

	i = -1;
	while (cmds && ++i < c->curr_cmdc)
	{
		a = (t_ast_node *)cmds->content;
		debug_print("\t### _do_log_commands got cmd:%s\n", p_get_cmd(a));
		if (0 != exec_fork_run(s, a, i, cmd_execute_full))	
			return (-1);
		waitchild(&exit_status, 1);
		debug_print("\t### _do_log_commands got exit:%d\n", exit_status);
		if (!ops)
			break ;
		debug_print("\t### _do_log_commands got op:%s\n", ops->content);
		if ((0 != exit_status && 0 == ft_strcmp(OP_ANDIF, ops->content)) \
			|| (0 == exit_status && 0 == ft_strcmp(OP_ORIF, ops->content)))
		{
			debug_print("Operator evalution stopping further exection\n");
			break ;
		}
		cmds = cmds->next;
		ops = ops->next;
	}
	c->curr_cmdc = i + 1;
	return (exit_status);
}

int cmd_exec_log(t_state *s, t_ast_node *node)
{
	t_cmd *cmd; 
	int exit_status;

	debug_print("\t### cmd_exec_log ###\n");
	cmd = get_cmd(s);
	if (!cmd)
		return (-1);
	cmd->curr_cmdc = p_get_log_cmdc(node);
	cmd->curr_node = node; // This might be problematic and we need a stack
	debug_print("\t got %d cmds\n", cmd->curr_cmdc);
	if (cmd->curr_cmdc < 2)
		return (-1);
	exit_status = _do_log_commands(s, p_get_log_cmds(node), cmd);
	if (exit_status < 0)
	{
		debug_print("\t### cmd_exec_log: got negative exit, returning\n");
		return (-1);
	}
		debug_print("\t### cmd_exec_log: got exit, returning\n");
	return (exit_status);
}
