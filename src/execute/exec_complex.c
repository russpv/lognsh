#include "execute_int.h"

/* Forks child and runs executor for 
 * higher-level AST nodes.
 */
int exec_fork_func(t_state *s, t_ast_node *node, execute_fn executor)
{
	pid_t pid;
	int status;
	
	pid = fork();
	if (0 > pid)
	{
		err("ERR exec_fork_func\n");
		return (-1);
	}
	else if (0 == pid)
	{
		int exit_status = executor(s, node);
		exit(exit_status); // No cleanup?
	}
	waitchild(&status, 1);
	return (get_exit_status(status));
}

