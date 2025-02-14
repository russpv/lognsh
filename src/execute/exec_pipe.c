#include "execute_int.h"

#define NO_APND false
#define DBGMSG_EPIPE_ANNOUNCE "Exec: exec_fork_run: got %dth\n"
#define DBGMSG_EPIPE_ANNOUNCE2 "Exec: _close_other_pipe_ends on %dth cmd.\n"
#define DBGMSG_EPIPE_CLOSED "Exec: Child %d: closed %d pipe ends (%d cmds)\n"
#define ERRMSG_EPIPE_REDIR "ERR redirect middle cmd stdin\n"
#define ERRMSG_EPIPE_REDIR2 "ERR redirect did nothing\n"

// depending on cmd, closes
// the right end of the pipe(s)
// i is the ith 0-indexed cmd
// ith - 1 pipe for read ends, ith for write ends
static int	_redirect_pipes(const t_cmd *c, int i)
{
	int	**fildes;
	int	r;

	fildes = (int **)c_get_fildes(c);
	r = -1;
	if (0 == i)
		r = redirect(&fildes[i][1], NULL, STDOUT_FILENO, NO_APND);
	else if (i < c_get_cmdc(c) - 1)
	{
		r = redirect(&fildes[i - 1][0], NULL, STDIN_FILENO, NO_APND);
		if (-1 == r)
			return (err(ERRMSG_EPIPE_REDIR), -1);
		r = redirect(&fildes[i][1], NULL, STDOUT_FILENO, NO_APND);
	}
	else if (i == c_get_cmdc(c) - 1)
		r = redirect(&fildes[i - 1][0], NULL, STDIN_FILENO, NO_APND);
	if (-1 == r)
		return (err(ERRMSG_EPIPE_REDIR2), -1);
	return (0);
}

/* Closes all the pipe ends for the i-th child
 * Critical to ensure SIGPIPE is sent to cmds with
 * no readers
 *
 * Closes all read ends except the prior pipe's
 * Closes all write ends except the ith pipe's
 *
 * For 1st child with heredoc, closes 1st pipe read end
 */
static int	_close_other_pipe_ends(const t_cmd *c, int i)
{
	int			pipe;
	int			counter;
	const int	**fildes = c_get_fildes(c);

	debug_print(DBGMSG_EPIPE_ANNOUNCE2, i);
	pipe = -1;
	counter = 0;
	while (++pipe < c_get_cmdc(c) - 1)
	{
		if (pipe != i)
		{
			close(fildes[pipe][1]);
			counter++;
		}
		if (pipe != i - 1)
		{
			close(fildes[pipe][0]);
			counter++;
		}
	}
	if (DEBUG)
		debug_print(DBGMSG_EPIPE_CLOSED, getpid(), counter, c_get_cmdc(c));
	return (1);
}

/* Child processing for pipe command
 * Forks, redirects to pipe, recursively
 * calls top level node executor
 * Calling command func will wait for exit status
 * Parent does not wait (until all pipe commands are forked).
 */
int	exec_pipe_fork_redirect_run(t_state *s, t_ast_node *node, int i,\
		t_execute_fn executor)
{
	pid_t	pid;
	int		exit_status;

	debug_print(DBGMSG_EPIPE_ANNOUNCE, i);
	if (SIGINT == g_last_signal)
		return (SIGINT_BEFORE_FORK);
	pid = fork();
	if (pid < 0)
	{
		err(ERRMSG_FORK);
		return (ERR_FORK);
	}
	else if (0 == pid)
	{
		reset_signal_handlers();
		_close_other_pipe_ends(get_cmd(s), i);
		_redirect_pipes(get_cmd(s), i);
		exit_status = executor(s, node);
		destroy_state(s);
		exit(exit_status);
	}
	return (0);
}
