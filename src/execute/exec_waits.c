
#include "execute.h"

#define DBGMSG_EXEC_EXITCODE "Exec: Child %d exited w/ stat:%d\n"
#define DBGMSG_EXEC_EXITSIG "Exec: Child %d exited by sig:%d\n"
#define DBGMSG_EXEC_DONE "Exec: All child processes have terminated\n"
#define DBGMSG_EXEC_WAITPID_ERROR "Exec: Waitpid failed on child:%d error:%d\n"

// Waits for the pid p child.
int	waitchildpid(int *status, pid_t p)
{
	pid_t	child_pid;
	int		raw_status;

	child_pid = waitpid(p, &raw_status, 0);
	if (child_pid > 0 && WIFEXITED(raw_status))
	{
		*status = exec_get_exit_status(raw_status);
		dprint(DBGMSG_EXEC_EXITCODE, child_pid, *status);
	}
	else if (child_pid > 0 && WIFSIGNALED(raw_status))
	{
		*status = 128 + WTERMSIG(raw_status);
		dprint(DBGMSG_EXEC_EXITSIG, child_pid, WTERMSIG(raw_status));
	}
	else if (child_pid < 0)
		return (perror(EMSG_WAITPID), ERR_WAITPID);
	dprint(DBGMSG_EXEC_DONE);
	return (0);
}

int	waitchildspid(int *status, t_exec *e)
{
	int	i;
	int	res;

	i = -1;
	while (++i < e->cmdc)
	{
		res = waitchildpid(status, e->pids[i]);
		if (0 != res)
			dprint(DBGMSG_EXEC_WAITPID_ERROR, e->pids[i], res);
		e->exit_codes[i] = *status;
	}
	return (0);
}

// Indiscriminantly waits for children and passes the last exit
// code returned.
int	waitchilds(int *status, int childc)
{
	int		i;
	pid_t	child_pid;
	int		raw_status;

	i = -1;
	while (++i < childc)
	{
		child_pid = waitpid(-1, &raw_status, 0);
		if (child_pid > 0 && WIFEXITED(raw_status))
		{
			*status = exec_get_exit_status(raw_status);
			dprint(DBGMSG_EXEC_EXITCODE, child_pid, *status);
		}
		else if (child_pid > 0 && WIFSIGNALED(raw_status))
		{
			*status = 128 + WTERMSIG(raw_status);
			dprint(DBGMSG_EXEC_EXITSIG, child_pid, WTERMSIG(raw_status));
		}
		else if (child_pid < 0)
			return (perror(EMSG_WAITPID), ERR_WAITPID);
	}
	dprint(DBGMSG_EXEC_DONE);
	return (0);
}

int	waitchild_sigint(int *status, pid_t child_pid)
{
	int	code;
	int	raw_status;

	code = 0;
	while (code <= 0)
	{
		code = waitpid(child_pid, &raw_status, WNOHANG);
		if (code > 0 && WIFEXITED(raw_status))
		{
			*status = exec_get_exit_status(raw_status);
			dprint(DBGMSG_EXEC_EXITCODE, child_pid, *status);
		}
		else if (code > 0 && WIFSIGNALED(raw_status))
		{
			*status = 128 + WTERMSIG(raw_status);
			dprint(DBGMSG_EXEC_EXITSIG, child_pid, WTERMSIG(raw_status));
		}
		else if (code < 0)
			return (perror(EMSG_WAITPID), ERR_WAITPID);
	}
	dprint(DBGMSG_EXEC_DONE);
	return (0);
}
