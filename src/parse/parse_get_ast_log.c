#include "parse_int.h"

int	p_get_log_cmdc(t_ast_node *a)
{
	if (p_get_type(a) != AST_NODE_LOG)
		return (-1);
	return (a->data.log.cmdc);
}

t_list	*p_get_log_cmds(t_ast_node *a)
{
	if (p_get_type(a) != AST_NODE_LOG)
		return (NULL);
	return (a->data.log.cmds);
}

t_list	*p_get_log_ops(t_ast_node *a)
{
	if (p_get_type(a) != AST_NODE_LOG)
		return (NULL);
	return (a->data.log.operators);
}
