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
