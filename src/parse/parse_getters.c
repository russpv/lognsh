#include "parse_int.h"

/* Command */

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


/* Logical */


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

/* Misc */


int	p_get_type(t_ast_node *a)
{
	if (!a)
		return (-1);
	return (a->type);
}

int	p_get_redc(t_ast_node *a)
{
	if (a->type != AST_NODE_CMD)
		return (-1);
	return (a->data.cmd.redc);
}

/* Returns a t_ast_node's t_list *redirs of t_redir_data */
t_list	*p_get_redirs(t_ast_node *a)
{
	if (a->type == AST_NODE_CMD)
	{
		dprint(_MOD_ ": p_get_redirs got: _%s_\n", a->data.cmd.name);
		return (a->data.cmd.redirs);
	}
	else if (a->type == AST_NODE_PROC)
	{
		dprint(_MOD_ ": p_get_redirs got a proc\n");
		return (a->data.proc.redirs);
	}
	return (NULL);
}

/* Returns a t_ast_node's t_list **redirs of t_redir_data for modification */
t_list	**p_get_redirs_ptr(t_ast_node *a)
{
	if (a->type == AST_NODE_CMD)
	{
		dprint(_MOD_ ": p_get_redirs_ptr got cmd: _%s_\n",
			a->data.cmd.name);
		return (&a->data.cmd.redirs);
	}
	else if (a->type == AST_NODE_PROC)
	{
		dprint(_MOD_ ": p_get_redirs_ptr got a proc\n");
		return (&a->data.proc.redirs);
	}
	return (NULL);
}

/* Pipe */

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

/* Procs */

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

/* Redirs */


bool	p_get_has_redgrptok(t_ast_node *n)
{
	if (!n)
		return (false);
	if (n->type == AST_NODE_CMD)
	{
		return (n->data.cmd.has_redgrouptoks);
	}
	else if (n->type == AST_NODE_PROC)
	{
		return (n->data.proc.has_redgrouptoks);
	}
	return (false);
}

bool	p_get_do_redir_exp(t_ast_node *n)
{
	if (!n)
		return (false);
	if (n->type == AST_NODE_CMD)
		return (n->data.cmd.do_redir_expansion);
	else if (n->type == AST_NODE_PROC)
		return (n->data.proc.do_redir_expansion);
	return (false);
}

bool	p_get_do_redir_glob(t_ast_node *n)
{
	if (!n)
		return (false);
	if (n->type == AST_NODE_CMD)
		return (n->data.cmd.do_redir_globbing);
	else if (n->type == AST_NODE_PROC)
		return (n->data.proc.do_redir_globbing);
	return (false);
}

char	*p_get_fn(const t_redir_data *red)
{
	if (!red)
		return (NULL);
	return (red->filename);
}
