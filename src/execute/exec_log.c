#include "execute_int.h"

/* Child processing for log command
 * Forks, recursively
 * calls top level node executor
 * Note: cannot return exit status of child to parent here
 */
int	exec_fork_run(t_state *s, t_ast_node *node, int i, execute_fn executor)
{
	pid_t pid;
	int exit_status;

	debug_print("exec_fork_run: got %dth\n", i);
	pid = fork();
	if (pid < 0)
	{
		err("ERR exec_fork_run\n");
		return (-1);
	}
	else if (0 == pid)
	{
		reset_signal_handlers();
		//destroy_state(s);
		exit_status = executor(s, node);
		exit(exit_status);
	}
	return (0);
}
