#include "execute_int.h"

#define DBGMSG_ELOGI_ANNOUNCE "Exec: exec_fork_run: got %dth\n"
#define EMSG_ELOGI_FORK "ERR exec_fork_run\n"

/* Child processing for log command
 * Forks, recursively
 * calls top level node executor
 * Note: cannot return exit status of child to parent here
 */
int	exec_fork_run(t_state *s, t_ast_node *node, int i, t_execute_fn executor)
{
	pid_t	pid;
	int		exit_status;

	debug_print(DBGMSG_ELOGI_ANNOUNCE, i);
	pid = fork();
	if (pid < 0)
	{
		perror(EMSG_ELOGI_FORK);
		return (ERR_FORK);
	}
	else if (0 == pid)
	{
		sig_reset_handlers();
		exit_status = executor(s, node);
		exit(exit_status);
	}
	return (0);
}
