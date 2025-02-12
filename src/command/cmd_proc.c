#include "command_int.h"

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
	log_print("Cmd: \t### cmd_exec_proc ###\n");
	i = -1;
	if (0 == cmdc || NULL == cmd)
		return (0);
	while (++i < cmdc)
	{
		// push onto s->command stack?
		node = cmd->content;
		debug_print("Cmd: \t### cmd_exec_proc got cmd type:%d\n",\
			p_get_type(node));
		exit_status = exec_fork_wait(s, node, cmd_execute_full);
		if (exit_status != 0)
			debug_print("Cmd: Command failed with exit status: %d\n",\
				exit_status);
		cmd = cmd->next;
	}
	log_print("Cmd: \t### cmd_exec_proc finished execution. ###\n");
	return (exit_status);
}
