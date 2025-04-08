#include "command.h"
#include "command_int.h"

#define LOGMSG_CPROC_ANNOUNCE "Cmd: \t### cmd_exec_proc ###\n"
#define DBGMSG_CPROC_GOT "Cmd: \t### cmd_exec_proc got subcmd type:%d\n"
#define DBGMSG_CPROC_BADEXEC "Cmd: Command failed with exit status: %d\n"
#define LOGMSG_CPROC_DONE "Cmd: \t### cmd_exec_proc finished execution. ###\n"

/* CMD EXEC PROC
 * Executes proc node.
 * Note: though it stores a t_list, proc can
 * only be one command
 */
int	cmd_exec_proc(t_state *s, t_ast_node *parent)
{
	const t_list	*cmd = p_get_proc_cmds(parent);
	t_ast_node		*cmd_node;
	t_cmd			*c;
	int				i;
	int				exit_status;
	t_cmd_fns		*cf;
	t_exec			*e;

	cmd_node = NULL;
	log_print(LOGMSG_CPROC_ANNOUNCE);
	if (p_get_type(parent) != AST_NODE_PROC)
		return (err("Invalid node type\n"), ERR_INVALID_CMD_TYPE);
	i = -1;
	if (0 == p_get_proc_cmdc(parent) || NULL == cmd)
		return (0);
	cf = init_cmd_fns(s);
	while (++i < p_get_proc_cmdc(parent))
	{
		cmd_node = cmd->content;
		debug_print(DBGMSG_CPROC_GOT, p_get_type(cmd_node));
		assert(p_get_args(parent) == NULL);
		c = get_cmd(s);
		e = exec_init(get_mem(s), p_get_proc_cmdc(parent), c, parent);
		exec_set_executor(e, cmd_execute_full);
		exit_status = proc_args_redirs(s, parent, c);
		if (ERR_REDIR == exit_status || NO_CMD == exit_status)
			return (destroy_exec(get_mem(s), e), destroy_cmd_fns(get_mem(s), cf), exit_status);
		exit_status = exec_fork_redirect_wait(s, cmd_node, e, cf);
		if (exit_status != 0)
			debug_print(DBGMSG_CPROC_BADEXEC, exit_status);
		cmd = cmd->next;
	}
	destroy_exec(get_mem(s), e);
	destroy_cmd_fns(get_mem(s), cf);
	log_print(LOGMSG_CPROC_DONE);
	return (exit_status);
}
