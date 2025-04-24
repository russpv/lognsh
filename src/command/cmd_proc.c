
#include "command.h"
#include "command_int.h"

#define LOGMSG_CPROC_ANNOUNCE "%s: \t### %s ###\n"
#define DBGMSG_CPROC_GOT "%s: \t### %s got subcmd type:%d\n"
#define DBGMSG_CPROC_BADEXEC "%s: Command failed with exit status: %d\n"
#define LOGMSG_CPROC_DONE "%s: \t### %s finished execution. ###\n"

static int	_do_args(t_state *s, t_ast_node *parent, t_cmd *c, t_cmd_fns *cf)
{
	int	exit_status;

	exit_status = proc_args_redirs(s, parent, c);
	if (ERR_REDIR == exit_status || NO_CMD == exit_status)
		destroy_cmd_fns(get_mem(s), cf);
	return (exit_status);
}

static int	_do_exec(t_state *s, t_cmd_fns *cf, t_ast_node *cmd_node, t_exec *e)
{
	int	exit_status;

	exit_status = exec_fork_redirect_wait(s, cmd_node, e, cf);
	if (exit_status != 0)
		dprint(DBGMSG_CPROC_BADEXEC, _MOD_, exit_status);
	destroy_exec(get_mem(s), e);
	return (exit_status);
}

static int	_execute(t_state *s, t_ast_node *parent, const t_list *cmd,
		t_cmd_fns *cf)
{
	int			i;
	t_cmd		*c;
	t_ast_node	*cmd_node;
	t_exec		*e;
	int			exit_status;

	i = -1;
	e = NULL;
	cmd_node = NULL;
	exit_status = 0;
	while (++i < p_get_proc_cmdc(parent))
	{
		cmd_node = cmd->content;
		dprint(DBGMSG_CPROC_GOT, _MOD_, __FUNCTION__, p_get_type(cmd_node));
		c = get_cmd(s);
		e = exec_init(get_mem(s), p_get_proc_cmdc(parent), c, parent);
		exec_set_executor(e, cmd_execute_full);
		exit_status = _do_args(s, parent, c, cf);
		if (ERR_REDIR == exit_status || NO_CMD == exit_status)
			return (destroy_exec(get_mem(s), e), exit_status);
		exit_status = _do_exec(s, cf, cmd_node, e);
		cmd = cmd->next;
	}
	return (exit_status);
}

/* CMD EXEC PROC
 * Executes proc node.
 * Note: though it stores a t_list, proc can
 * only be one command
 */
int	cmd_exec_proc(t_state *s, t_ast_node *parent)
{
	const t_list	*cmd = p_get_proc_cmds(parent);
	int				exit_status;
	t_cmd_fns		*cf;

	lgprint(LOGMSG_CPROC_ANNOUNCE, _MOD_, __FUNCTION__);
	if (p_get_type(parent) != AST_NODE_PROC)
		return (err("Invalid node type\n"), ERR_INVALID_CMD_TYPE);
	if (0 == p_get_proc_cmdc(parent) || NULL == cmd)
		return (0);
	cf = init_cmd_fns(s);
	exit_status = _execute(s, parent, cmd, cf);
	if (0 != exit_status)
		return (exit_status);
	destroy_cmd_fns(get_mem(s), cf);
	lgprint(LOGMSG_CPROC_DONE, _MOD_, __FUNCTION__);
	return (exit_status);
}
