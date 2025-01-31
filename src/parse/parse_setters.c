#include "parse_int.h"

// Sets cmd name. Assumes new c-string cmd
int	p_set_cmd(t_ast_node *a, const char *cmd)
{
	if (a->type != AST_NODE_CMD || !cmd)
		return (-1);
	a->data.cmd.name = ft_strdup(cmd);
	if (!a->data.cmd.name)
		return (err("ERR p_set_cmd malloc\n"), -1);
	return (0);
}
