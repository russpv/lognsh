#include "parse_int.h"

/* Returns the cmd name of an AST_NODE_CMD node */
char	*p_get_cmd(t_ast_node *a)
{
	if (a->type != AST_NODE_CMD)
		return (NULL);
	return (a->data.cmd.name);
}

/* Returns AST command node's POINTER to arguments llist */
t_list	**p_get_args(t_ast_node *a)
{
	if (a->type != AST_NODE_CMD)
		return (NULL);
	return (&a->data.cmd.args);
}

int	p_get_argc(t_ast_node *a)
{
	if (a->type != AST_NODE_CMD)
		return (-1);
	return (a->data.cmd.argc);
}

bool	p_get_expansion(t_ast_node *a)
{
	if (p_get_type(a) != AST_NODE_CMD)
		return (NULL);
	return (a->data.cmd.do_expansion);
}

bool	p_get_grouptok(t_ast_node *a)
{
	if (p_get_type(a) != AST_NODE_CMD)
		return (NULL);
	return (a->data.cmd.has_arggrouptoks);
}

int	p_get_redc(t_ast_node *a)
{
	if (a->type != AST_NODE_CMD)
		return (-1);
	return (a->data.cmd.redc);
}
