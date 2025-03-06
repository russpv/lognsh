#include "command_int.h"

#define LOGMSG_CPIPE_ANNOUNCE "Cmd: \t### cmd_exec_pipe ###\n"
#define DBGMSG_CPIPE_GOT "Cmd: \t got %d cmds\n"
#define EMSG_CPIPE_PIPE "ERR pipe creation\n"

static int	_setup_pipes(t_cmd *c)
{
	int res;
	res = exec_create_pipes(&c->fildes, c->curr_cmdc);
	if (0 != res)
		return (err(EMSG_CPIPE_PIPE), res);
	print_pipes(c);
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
	if (0 != waitchild(&status, c->curr_cmdc))
		return (ERR_WAITPID);
	set_exit_status(s, get_exit_status(status));
	return (get_exit_status(status));
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
	res = _setup_pipes(cmd);
	if (0 != res)
		return (res);
	res = _do_pipe_commands(s, p_get_pipe_cmds(pipe), cmd);
	if (0 != res)
		return (res);
	return (_wait_all(s, cmd));
}
