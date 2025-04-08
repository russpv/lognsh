#include "execute.h"

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
