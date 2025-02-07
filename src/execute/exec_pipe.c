#include "execute_int.h"

#define NO_APND false

/* Frees allocated pipes */
void free_pipes(int **fildes, int count)
{
    int i = 0;
    while (i < count)
        free(fildes[i++]);
    free(*fildes);
    *fildes = NULL;
}

/* Allocates pipe fd's to null terminated array */
int	exec_create_pipes(int ***fildes, int cmd_count)
{
	int	i;

	i = 0;
	*fildes = (int **)malloc((sizeof(int *)) * (size_t)(cmd_count));
	if (!(*fildes))
		return(err("ERR malloc"), -1);
	(*fildes)[cmd_count - 1] = NULL;
	while (i < cmd_count - 1)
	{
		(*fildes)[i] = (int *)malloc(2 * sizeof(int));
		if (!(*fildes)[i])
		{
			free_pipes((*fildes), i);
			return(err("malloc"), -1);
		}
		if (pipe((*fildes)[i]) < 0)
		{
			free_pipes((*fildes), i);
			return(err("pipe"), -1);
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
		close(fildes[i][0]);
		close(fildes[i][1]);
	}
	return (0);
}

// depending on cmd, closes 
// the right end of the pipe(s)
// i is the ith 0-indexed cmd
// ith - 1 pipe for read ends, ith for write ends
static int	_redirect_pipes(const t_cmd *c, int i)
{
	int **fildes;
	int r;

	fildes = (int **)c_get_fildes(c);
	r = -1;
	if (0 == i)
		r = redirect(&fildes[i][1], NULL, STDOUT_FILENO, NO_APND);
	else if (i < c_get_cmdc(c) - 1)
	{ 
		r = redirect(&fildes[i - 1][0], NULL, STDIN_FILENO, NO_APND);
		if (-1 == r)
			return (err("ERR redirect middle cmd stdin\n"), -1);
		r = redirect(&fildes[i][1], NULL, STDOUT_FILENO, NO_APND);
	}
	else if (i == c_get_cmdc(c) - 1)
		r = redirect(&fildes[i - 1][0], NULL, STDIN_FILENO, NO_APND);
	if (-1 == r)
		return (err("ERR redirect did nothing\n"), -1);
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
	int	pipe;
	int	counter;
	const int **fildes = c_get_fildes(c);

	debug_print("_close_other_pipe_ends on %dth cmd.\n", i);
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
	debug_print("Child %d: closed %d pipe ends (%d cmds)\n", getpid(), counter, \
		c_get_cmdc(c));
	return (1);
}

/* Child processing for pipe command
 * Forks, redirects to pipe, recursively
 * calls top level node executor
 */
int	exec_fork_redirect_run(t_state *s, t_ast_node *node, int i, execute_fn executor)
{
	pid_t pid;
	int exit_status;

	debug_print("exec_fork_func_child: got %dth\n", i);
	pid = fork();
	if (pid < 0)
	{
		err("ERR exec_fork_func\n");
		return (-1);
	}
	else if (0 == pid)
	{
		_close_other_pipe_ends(get_cmd(s), i);
		_redirect_pipes(get_cmd(s), i);
		exit_status = executor(s, node);
		destroy_state(s);
		exit(exit_status);
	}
	return (0);
}
