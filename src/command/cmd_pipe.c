#include "command_int.h"

static int	_setup_pipes(t_cmd *c)
{
	if (0 != exec_create_pipes(&c->fildes, c->curr_cmdc))
		return (err("ERR pipe creation\n"), -1);
 	for (int i = 0; i < c->curr_cmdc - 1; i++)
        debug_print("Cmd: \tPipe %d: read fd=%d, write fd=%d\n", i, c->fildes[i][0], c->fildes[i][1]);
    return (0);
}

// Returns exit status
//TODO cleanup
static int _wait_and_clean(t_cmd *c)
{
	int status;

	if (0 != exec_close_pipes(c->fildes, c->curr_cmdc))
		return (-1);
	waitchild(&status, c->curr_cmdc);
	return (get_exit_status(status));
}

static int	_do_pipe_commands(t_state *s, t_list *cmds, t_cmd *c)
{
	t_ast_node *a;
	int i;

	i = -1;
	while (cmds && ++i < c->curr_cmdc)
	{
		a = (t_ast_node *)cmds->content;
		if (0 != exec_pipe_fork_redirect_run(s, a, i, cmd_execute_full))
			return (-1);
		cmds = cmds->next;
	}
	return (0);
}

/* Executes commands within pipeline node
 * Not guaranteed to have forked, so no cleanup done.
 */
int cmd_exec_pipe(t_state *s, t_ast_node *pipe)
{
	t_cmd *cmd; 
	
	debug_print("Cmd: \t### cmd_exec_pipe ###\n");
	cmd = get_cmd(s);
	if (!cmd)
		return (-1);
	cmd->curr_cmdc = p_get_pipe_cmdc(pipe);
	debug_print("Cmd: \t got %d cmds\n", cmd->curr_cmdc);
	if (cmd->curr_cmdc < 2)
		return (-1);
	if (0 != _setup_pipes(cmd))
		return (-1);
	if (0 != _do_pipe_commands(s, p_get_pipe_cmds(pipe), cmd))
		return (-1);
	return (_wait_and_clean(cmd));
}
