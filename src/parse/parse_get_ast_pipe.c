#include "parse_int.h"

int	p_get_pipe_cmdc(t_ast_node *a)
{
	if (p_get_type(a) != AST_NODE_PIPELINE)
		return (-1);
	return (a->data.pipe.cmdc);
}

t_list	*p_get_pipe_cmds(t_ast_node *a)
{
	if (p_get_type(a) != AST_NODE_PIPELINE)
		return (NULL);
	return (a->data.pipe.cmds);
}
