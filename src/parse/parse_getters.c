#include "parse_int.h"

/* Returns the cmd name of an AST_NODE_CMD node */
char *p_get_cmd(t_ast_node *a)
{
	if (a->type != AST_NODE_CMD)
		return (NULL);
	debug_print("p_get_cmd got: _%s_\n", a->data.cmd.name);
	return (a->data.cmd.name);
}

int p_get_type(t_ast_node *a)
{
	return (a->type);
}

t_list **p_get_args(t_ast_node *a)
{
	if (a->type != AST_NODE_CMD)
		return (NULL);
	debug_print("p_get_args got: _%s_\n", a->data.cmd.name);
	return (&a->data.cmd.args);
}

/* Returns the llist of t_arg_data node content */
/*
t_list *p_get_args(t_ast_node *a)
{
	if (a->type != AST_NODE_CMD)
		return (NULL);
	debug_print("p_get_args got: _%s_\n", a->data.cmd.name);
	return (a->data.cmd.args);
}*/

t_list *p_get_redirs(t_ast_node *a)
{
	if (a->type != AST_NODE_CMD)
		return (NULL);
	debug_print("p_get_redirs got: _%s_\n", a->data.cmd.name);
	return (a->data.cmd.redirs);
}

int		p_get_argc(t_ast_node *a)
{
	if (a->type != AST_NODE_CMD)
		return (-1);
	debug_print("p_get_argc got: _%s_\n", a->data.cmd.name);
	return (a->data.cmd.argc);
}

// TODO change to actual argv
char **p_get_argv(t_ast_node *a)
{
	char **args;

	args = NULL;
	if (!a)
		return (NULL);
	if (p_get_type(a) != AST_NODE_CMD)
		return (NULL);
	if (a->data.cmd.argc > 0)
	{
		args = list_to_array(a->data.cmd.args, a->data.cmd.argc);
	}
	return (args);
}

bool	p_get_expansion(t_ast_node *a)
{
	if (p_get_type(a) != AST_NODE_CMD)
		return (NULL);
	return (a->data.cmd.do_expansion);
}

int	p_get_proc_cmdc(t_ast_node *a)
{
	if (p_get_type(a) != AST_NODE_PROC)
		return (-1);
	return (a->data.proc.cmdc);
}

t_list	*p_get_proc_cmds(t_ast_node *a)
{
	if (p_get_type(a) != AST_NODE_PROC)
		return (NULL);
	return (a->data.proc.cmds);
}

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
