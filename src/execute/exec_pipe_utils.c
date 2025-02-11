#include "execute.h"

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
	*fildes = (int **)malloc((sizeof(int *)) * (size_t)(cmd_count));
	if (!(*fildes))
		return (err("ERR malloc"), -1);
	(*fildes)[cmd_count - 1] = NULL;
	while (i < cmd_count - 1)
	{
		(*fildes)[i] = (int *)malloc(2 * sizeof(int));
		if (!(*fildes)[i])
		{
			free_pipes((*fildes), i);
			return (err("malloc"), -1);
		}
		if (pipe((*fildes)[i]) < 0)
		{
			free_pipes((*fildes), i);
			return (err("pipe"), -1);
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
