
#include "execute_int.h"

/* Interpret waitpid() exit status (signals ignored here) */
int	exec_get_exit_status(int status)
{
	int	exit_status;
	int	signal_number;

	exit_status = 0;
	signal_number = 0;
	if (WIFEXITED(status))
	{
		exit_status = WEXITSTATUS(status);
		return (exit_status);
	}
	else if (WIFSIGNALED(status))
	{
		signal_number = WTERMSIG(status);
		return (signal_number);
	}
	return (-1);
}

int	handle_exit(t_state *s, int code)
{
	cprintf(MAGENTA, "got code:%d\n", code);
	if (EX_HAPPY == code)
		return (EX_HAPPY);
	else if (ERR_REDIR == code)
		return (set_exit_status(s, EX_EREDIR), EX_ERNDM);
	else
		return (set_exit_status(s, code), code);
}
