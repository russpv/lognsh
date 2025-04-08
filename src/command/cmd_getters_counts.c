#include "command_int.h"

int	c_get_cmdc(const t_cmd *c)
{
	if (!c)
		return (-1);
	return (c->curr_cmdc);
}

int	c_get_redc(const t_cmd *c)
{
	if (!c)
		return (-1);
	return (c->redc);
}
