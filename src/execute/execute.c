#include "execute_int.h"

/* This wait()'s for all child processes; called by parent
 * Note: assuming waitpid() behaves irrespective of actual
 * number of children.
 */
void	waitchild(int *status, int childc)
{
	int		i;
	pid_t	child_pid;

	i = -1;
	while (++i < childc)
	{
		child_pid = waitpid(-1, status, 0);
		{
			if (child_pid > 0 && WIFEXITED(*status))
				debug_print("Exec: Child %d exited w/ stat:%d\n", child_pid,
					WEXITSTATUS(*status));
			else if (child_pid > 0 && WIFSIGNALED(*status))
				debug_print("Exec: Child %d exited by sig:%d\n", child_pid,
					WTERMSIG(*status));
		}
	}
	debug_print("Exec: All child processes have terminated\n");
}

/* Interpret waitpid() exit status (signals ignored here) */
int	get_exit_status(int status)
{
	int	exit_status;
	int	signal_number;

	exit_status = 0;
	signal_number = 0;
	if (WIFEXITED(status))
		exit_status = WEXITSTATUS(status);
	else if (WIFSIGNALED(status))
		signal_number = WTERMSIG(status);
	(void)signal_number;
	return (exit_status);
}
