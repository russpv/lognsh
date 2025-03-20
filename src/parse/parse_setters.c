#include "parse_int.h"

// Sets cmd name. Assumes new c-string cmd
int	p_set_cmd(t_mem_mgr *m, t_ast_node *a, const char *cmd)
{
	if (a->type != AST_NODE_CMD || !cmd)
		return (-1);
	a->data.cmd.name = ft_strdup_tmp(m, cmd);
	if (!a->data.cmd.name)
		exit_clean(&m->list, ENOMEM, __FUNCTION__, EMSG_MALLOC);
	return (0);
}

// Adds amt to redirect count for multiple AST node types
int	p_update_redc(t_ast_node *a, int amt)
{
	if (AST_NODE_CMD == a->type)
	{
		a->data.cmd.redc += amt;
		return (a->data.cmd.redc);
	}
	else if (AST_NODE_PROC == a->type)
	{
		a->data.proc.redc += amt;
		return (a->data.proc.redc);
	}
	return (ERR_ARGS);
}
// Adds amt to argument count for command node type
int	p_update_argc(t_ast_node *a, int amt)
{
	if (AST_NODE_CMD == a->type)
	{
		a->data.cmd.argc += amt;
		return (a->data.cmd.argc);
	}
	return (ERR_ARGS);
}
