#include "command_int.h"

char	*c_getfullpath(t_cmd *c)
{
	return (c->fullpath);
}

char	**c_get_argv(t_cmd *c)
{
	return (c->argv);
}

t_stack	*c_getctxt(t_cmd *c)
{
	return (c->st);
}

t_ast_node	*c_getnode(t_cmd *c)
{
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

int	c_get_argc(const t_cmd *c)
{
	if (!c)
		return (-1);
	return (c->argc);
}
/*
t_list	*c_get_redirs(t_ast_node *node)
{
	if (!node)
		return (NULL);
	if (AST_NODE_CMD == p_get_type(node))
	{
		return (p_get_red)
	}
	else if (AST_NODE_PROC == p_get_type(node))
	{

	}
	return (NULL);
}
*/