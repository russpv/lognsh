#include "command_int.h"

char	*c_get_fullpath(t_cmd *c)
{
	if (!c)
		return (NULL);
	return (c->fullpath);
}

t_int_stack	*c_get_ctxtst(t_cmd *c)
{
	if (!c)
		return (NULL);
	return (c->st);
}

t_ast_node	*c_get_node(t_cmd *c)
{
	if (!c)
		return (NULL);
	return (c->curr_node);
}

const int	**c_get_fildes(const t_cmd *c)
{
	if (!c)
		return (NULL);
	return ((const int **)c->fildes);
}

t_list	*c_get_redirs(const t_cmd *c)
{
	if (!c)
		return (NULL);
	return (c->redirs);
}
