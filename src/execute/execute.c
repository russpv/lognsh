#include "execute_int.h"

#define DBGMSG_EXEC_EXITCODE _MOD_ ": Child %d exited w/ stat:%d\n"
#define DBGMSG_EXEC_EXITSIG _MOD_ ": Child %d exited by sig:%d\n"
#define DBGMSG_EXEC_DONE _MOD_ ": All child processes have terminated\n"

/* This wait()'s for all child processes; called by parent
 * Note: assuming waitpid() behaves irrespective of actual
 * number of children.
 */
/*int	waitchilds(int *status, int childc)
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
				return (perror(EMSG_WAITPID), ERR_WAITPID);
		}
	}
	debug_print(DBGMSG_EXEC_DONE);
	return (0);
}*/

int	waitchildpid(int *status, pid_t p)
{
	pid_t	child_pid;
	int	raw_status;

	child_pid = waitpid(p, &raw_status, 0);
	if (child_pid > 0 && WIFEXITED(raw_status))
	{
		*status = exec_get_exit_status(raw_status);  // Sets to 1
		debug_print(DBGMSG_EXEC_EXITCODE, child_pid, *status);
	}
	else if (child_pid > 0 && WIFSIGNALED(raw_status))
	{
		*status = 128 + WTERMSIG(raw_status);
		debug_print(DBGMSG_EXEC_EXITSIG, child_pid, WTERMSIG(raw_status));
	}
	else if (child_pid < 0)
		return (perror(EMSG_WAITPID), ERR_WAITPID);
	debug_print(DBGMSG_EXEC_DONE);
	return (0);
}

int	waitchilds(int *status, int childc)
{
	int	i;
	pid_t	child_pid;
	int	raw_status;

	i = -1;
	while (++i < childc)
	{
		child_pid = waitpid(-1, &raw_status, 0);
		if (child_pid > 0 && WIFEXITED(raw_status))
		{
			*status = exec_get_exit_status(raw_status);  // Sets to 1
			debug_print(DBGMSG_EXEC_EXITCODE, child_pid, *status);
		}
		else if (child_pid > 0 && WIFSIGNALED(raw_status))
		{
			*status = 128 + WTERMSIG(raw_status);
			debug_print(DBGMSG_EXEC_EXITSIG, child_pid, WTERMSIG(raw_status));
		}
		else if (child_pid < 0)
			return (perror(EMSG_WAITPID), ERR_WAITPID);
	}
	debug_print(DBGMSG_EXEC_DONE);
	return (0);
}

/* Interpret waitpid() exit status (signals ignored here) */
int	exec_get_exit_status(int status)
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

int	waitchild_sigint(int *status, pid_t child_pid)
{
	int code;
	int	raw_status;

	code = 0;
	while (code <= 0)
	{
		code = waitpid(child_pid, &raw_status, WNOHANG);
		if (code > 0 && WIFEXITED(raw_status))
		{
			*status = exec_get_exit_status(raw_status);  // Sets to 1
			debug_print(DBGMSG_EXEC_EXITCODE, child_pid, *status);
		}
		else if (code > 0 && WIFSIGNALED(raw_status))
		{
			*status = 128 + WTERMSIG(raw_status);
			debug_print(DBGMSG_EXEC_EXITSIG, child_pid, WTERMSIG(raw_status));
		}
		else if (code < 0)
			return (perror(EMSG_WAITPID), ERR_WAITPID);
	}
	debug_print(DBGMSG_EXEC_DONE);
	return (0);
}

int	handle_exit(t_state *s, int code)
{
	fprintf(stderr, "got code:%d\n", code);
	if (EX_HAPPY == code)
		return (EX_HAPPY);
	else if (ERR_REDIR == code)
		return (set_exit_status(s, EX_EREDIR), EX_ERNDM); // Basb likes dumb codes
	else
		return (set_exit_status(s, code), code);
}
