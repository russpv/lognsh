#include "execute.h"

#define EMSG_EPIPE_FORK "ERR exec_fork_run\n"

/* Frees allocated pipes */
void	free_pipes(int **fildes, int count)
{
	int	i;

	i = 0;
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
	if (cmd_count < 2)
		return (ERR_INSUFFICIENT_CMDS);
	*fildes = (int **)malloc((sizeof(int *)) * (size_t)(cmd_count));
	if (!(*fildes))
		return (err(EMSG_MALLOC), ERR_MEM);
	(*fildes)[cmd_count - 1] = NULL;
	while (i < cmd_count - 1)
	{
		(*fildes)[i] = (int *)malloc(2 * sizeof(int));
		if (!(*fildes)[i])
		{
			free_pipes((*fildes), i);
			return (err(EMSG_MALLOC), ERR_MEM);
		}
		if (pipe((*fildes)[i]) < 0)
		{
			free_pipes((*fildes), i);
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
			return(perror(EMSG_CLOSE), ERR_CLOSE);
		if (0 != close(fildes[i][1]))
			return(perror(EMSG_CLOSE), ERR_CLOSE); 
	}
	return (0);
}
