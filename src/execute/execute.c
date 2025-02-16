#include "execute_int.h"

#define DBGMSG_EXEC_EXITCODE "Exec: Child %d exited w/ stat:%d\n"
#define DBGMSG_EXEC_EXITSIG "Exec: Child %d exited by sig:%d\n"
#define DBGMSG_EXEC_DONE "Exec: All child processes have terminated\n"

/* This wait()'s for all child processes; called by parent
 * Note: assuming waitpid() behaves irrespective of actual
 * number of children.
 */
int	waitchild(int *status, int childc)
{
	int		i;
	pid_t	child_pid;

	i = -1;
	while (++i < childc)
	{
		child_pid = waitpid(-1, status, 0);
		{
			if (child_pid > 0 && WIFEXITED(*status))
				debug_print(DBGMSG_EXEC_EXITCODE, child_pid,
					WEXITSTATUS(*status));
			else if (child_pid > 0 && WIFSIGNALED(*status))
				debug_print(DBGMSG_EXEC_EXITSIG, child_pid,
					WTERMSIG(*status));
			else if (child_pid < 0)
				return (perror(ERRMSG_WAITPID), ERR_WAITPID);
		}
	}
	debug_print(DBGMSG_EXEC_DONE);
	return (0);
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
