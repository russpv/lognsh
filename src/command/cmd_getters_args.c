
#include "command_int.h"

int	c_get_argvc(const t_cmd *c)
{
	if (!c)
		return (-1);
	return (c->argvc);
}

char	**c_get_argv(t_cmd *c)
{
	return (c->argv);
}
