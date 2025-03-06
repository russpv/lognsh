#include "command_int.h"

char	*c_get_fullpath(t_cmd *c)
{
	return (c->fullpath);
}

t_int_stack	*c_get_ctxtst(t_cmd *c)
{
	return (c->st);
}

t_ast_node	*c_get_node(t_cmd *c)
{
	assert(NULL != c);
	return (c->curr_node);
}

const int	**c_get_fildes(const t_cmd *c)
{
	return ((const int **)c->fildes);
}

int	c_get_cmdc(const t_cmd *c)
{
	if (!c)
		return (-1);
	return (c->curr_cmdc);
}
