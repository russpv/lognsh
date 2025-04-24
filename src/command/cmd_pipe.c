
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
	print_pipes(c);
	return (0);
}

/* Closes all pipes in the parent,
 * waits for all pipeline children,
 * then sets and returns exit status of
 * last pipeline command.
 * destroys t_exec
 */
static int	_wait_all(t_state *s, t_cmd *c, t_exec *e)
{
	int	status;

	exec_close_pipes(c->fildes, c->curr_cmdc);
	if (0 != waitchildspid(&status, e))
		return (ERR_WAITPID);
	set_exit_status(s, status);
	destroy_exec(get_mem(s), e);
	return (status);
}

static int	_do_pipe_commands(t_state *s, t_list *cmds, t_cmd *c, t_exec *e)
{
	t_ast_node	*a;
	int			i;
	int			res;

	i = -1;
	exec_set_executor(e, cmd_execute_full);
	while (cmds && ++i < c->curr_cmdc)
	{
		a = (t_ast_node *)cmds->content;
		res = exec_pipe_fork_redirect_run(s, a, i, e);
		if (res < 0)
			return (res);
		cmds = cmds->next;
	}
	return (res);
}

/* Executes commands within pipeline node.
 * Returns the last pipeline command's exit code.
 * Not guaranteed to have forked, so no cleanup done.
 */
int	cmd_exec_pipe(t_state *s, t_ast_node *pipe)
{
	t_cmd	*cmd;
	int		res;
	t_exec	*e;

	lgprint(LOGMSG_CPIPE_ANNOUNCE);
	cmd = get_cmd(s);
	if (!cmd)
		return (ERR_ARGS);
	cmd->curr_cmdc = p_get_pipe_cmdc(pipe);
	dprint(DBGMSG_CPIPE_GOT, cmd->curr_cmdc);
	if (cmd->curr_cmdc < 2)
		return (ERR_INSUFFICIENT_CMDS);
	e = exec_init(get_mem(s), cmd->curr_cmdc, NULL, NULL);
	res = _setup_pipes(get_mem(s), cmd);
	if (0 != res)
		return (res);
	_do_pipe_commands(s, p_get_pipe_cmds(pipe), cmd, e);
	return (_wait_all(s, cmd, e));
}
