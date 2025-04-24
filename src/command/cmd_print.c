
#include "command_int.h"

void	print_pipes(t_cmd *c)
{
	int	i;

	if (c == NULL || c->fildes == NULL)
		return (dprint("Error: fildes is NULL\n"));
	i = -1;
	while (++i < c->curr_cmdc - 1)
		dprint("Cmd: \tPipe %d: read fd=%d, write fd=%d\n", i, c->fildes[i][0],
			c->fildes[i][1]);
}
