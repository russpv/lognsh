#include "command_int.h"

// Even though it stores a t_list, it can 
// only be one command
int cmd_exec_proc(t_state *s, t_ast_node *a)
{
	debug_print("\t### cmd_exec_proc ###\n");
	const int cmdc = p_get_proc_cmdc(a);
	const t_list *cmd = p_get_proc_cmds(a);
	t_ast_node *node = NULL;
	int i;

	i = -1;
	if (0 == cmdc || NULL == cmd)
		return (0);
	while (++i < cmdc)
	{
		//push onto stack?
		node = cmd->content;
		exec_fork_func(s, node, cmd_execute_full);
		cmd = cmd->next;
	}
	return (0);
}
