#include "command_int.h"

int	c_get_argc(const t_cmd *c)
{
	if (!c)
		return (-1);
	return (c->argc);
}

char	**c_get_argv(t_cmd *c)
{
	return (c->argv);
}
