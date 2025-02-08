#include "parse_int.h"

/* Returns the cmd name of an AST_NODE_CMD node */
char *p_get_cmd(t_ast_node *a)
{
	if (a->type != AST_NODE_CMD)
		return (NULL);
	debug_print("Parser: p_get_cmd got: _%s_\n", a->data.cmd.name);
	return (a->data.cmd.name);
}

t_list **p_get_args(t_ast_node *a)
{
	if (a->type != AST_NODE_CMD)
		return (NULL);
	debug_print("Parser: p_get_args got: _%s_\n", a->data.cmd.name);
	return (&a->data.cmd.args);
}

int		p_get_argc(t_ast_node *a)
{
	if (a->type != AST_NODE_CMD)
		return (-1);
	debug_print("Parser: p_get_argc got: _%s_\n", a->data.cmd.name);
	return (a->data.cmd.argc);
}

bool	p_get_expansion(t_ast_node *a)
{
	if (p_get_type(a) != AST_NODE_CMD)
		return (NULL);
	return (a->data.cmd.do_expansion);
}
