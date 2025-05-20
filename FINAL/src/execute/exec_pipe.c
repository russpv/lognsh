#include "execute_int.h"

#define DBGMSG_EPIPE_ANNOUNCE "Exec: exec_fork_run: got %dth\n"
#define DBGMSG_EPIPE_ANNOUNCE2 "Exec: _close_other_pipe_ends on %dth cmd.\n"
#define DBGMSG_EPIPE_CLOSED "Exec: Child %d: closed %d pipe ends (%d cmds)\n"
#define EMSG_EPIPE_REDIR "ERR redirect middle cmd stdin\n"
#define EMSG_EPIPE_REDIR2 "ERR redirect did nothing\n"
#define EMSG_EPIPE_FORK "ERR exec_fork_run\n"

/* Frees allocated pipes */
static void	_free_pipes(t_mem_mgr *m, int **fildes, int count)
{
	int	i;

	i = 0;
	while (i < count)
		m->dealloc(&m->list, fildes[i++]);
	m->dealloc(&m->list, *fildes);
	*fildes = NULL;
}

/* Allocates pipe fd's to null terminated array */
int	exec_create_pipes(t_mem_mgr *m, int ***fildes, int cmd_count)
{
	int	i;

	i = 0;
	if (cmd_count < 2)
		return (err("Insufficient cmds\n"), ERR_INSUFFICIENT_CMDS);
	*fildes = (int **)m->f(&m->list, (sizeof(int *)) * (size_t)(cmd_count));
	if (!(*fildes))
		exit_clean(&m->list, ENOMEM, __FUNCTION__, EMSG_MALLOC);
	(*fildes)[cmd_count - 1] = NULL;
	while (i < cmd_count - 1)
	{
		(*fildes)[i] = (int *)m->f(&m->list, 2 * sizeof(int));
		if (!(*fildes)[i])
			exit_clean(&m->list, ENOMEM, __FUNCTION__, EMSG_MALLOC);
		if (pipe((*fildes)[i]) < 0)
		{
			_free_pipes(m, (*fildes), i);
			return (perror(EMSG_PIPE), ERR_PIPE);
		}
		i++;
	}
	return (0);
}

/* Closes all pipe fd's. */
int	exec_close_pipes(int **fildes, int cmd_count)
{
	int	i;

	i = -1;
	while (++i < cmd_count - 1)
	{
		if (0 != close(fildes[i][0]))
			return (perror(EMSG_CLOSE), ERR_CLOSE);
		if (0 != close(fildes[i][1]))
			return (perror(EMSG_CLOSE), ERR_CLOSE);
	}
	return (0);
}

// depending on cmd, closes
// the right end of the pipe(s)
// i is the ith 0-indexed cmd
// ith - 1 pipe for read ends, ith for write ends
static int	_redirect_pipes(t_state *s, int i)
{
	const t_cmd	*c = get_cmd(s);
	int			**fildes;
	int			res;

	fildes = (int **)(get_cmd_fns(s))->c_get_fildes(c);
	res = -1;
	if (0 == i)
		res = redirect(&fildes[i][1], NULL, STDOUT_FILENO, NO_APND);
	else if (i < (get_cmd_fns(s))->c_get_cmdc(c) - 1)
	{
		res = redirect(&fildes[i - 1][0], NULL, STDIN_FILENO, NO_APND);
		if (-1 == res)
			return (err(EMSG_EPIPE_REDIR), -1);
		res = redirect(&fildes[i][1], NULL, STDOUT_FILENO, NO_APND);
	}
	else if (i == (get_cmd_fns(s))->c_get_cmdc(c) - 1)
		res = redirect(&fildes[i - 1][0], NULL, STDIN_FILENO, NO_APND);
	if (res < 0)
		return (err(EMSG_EPIPE_REDIR2), -1);
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
static int	_close_other_pipe_ends(t_state *s, int i)
{
	int			pipe;
	const t_cmd	*c = get_cmd(s);
	const int	**fildes = (get_cmd_fns(s))->c_get_fildes(c);

	dprint(DBGMSG_EPIPE_ANNOUNCE2, i);
	pipe = -1;
	while (++pipe < (get_cmd_fns(s))->c_get_cmdc(c) - 1)
	{
		if (pipe != i)
		{
			if (0 != close(fildes[pipe][1]))
				return (perror(EMSG_CLOSE), ERR_CLOSE);
		}
		if (pipe != i - 1)
		{
			if (0 != close(fildes[pipe][0]))
				return (perror(EMSG_CLOSE), ERR_CLOSE);
		}
	}
	if (DEBUG)
		dprint(DBGMSG_EPIPE_CLOSED, getpid(), pipe + 1,
			(get_cmd_fns(s))->c_get_cmdc(c));
	return (1);
}

/* Child processing for pipe command
 * Forks, redirects to pipe, recursively.
 * Returns the child's pid, or negative value if error.
 * calls top level node executor
 * Calling command func will wait for exit status
 * Parent does not wait (until all pipe commands are forked).
 */
int	exec_pipe_fork_redirect_run(t_state *s, t_ast_node *node, int i, t_exec *e)
{
	pid_t	pid;
	int		exit_status;

	dprint(DBGMSG_EPIPE_ANNOUNCE, i);
	if (SH_SIGINT == g_last_signal)
		return (SIGINT_BEFORE_FORK);
	pid = fork();
	if (pid < 0)
	{
		perror(EMSG_FORK);
		return (-ERR_FORK);
	}
	else if (0 == pid)
	{
		sig_reset_handlers();
		_close_other_pipe_ends(s, i);
		_redirect_pipes(s, i);
		exit_status = e->executor(s, node);
		exit_status = handle_exit(s, exit_status);
		destroy_state(&s);
		exit(exit_status);
	}
	e->pids[i] = pid;
	return (pid);
}
