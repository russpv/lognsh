#include "parse_int.h"

void	p_set_has_redgrouptoks(t_ast_node *n, bool val)
{
	if (!n)
		return ;
	if (n->type == AST_NODE_CMD)
	{
		n->data.cmd.has_redgrouptoks = val;
	}
	else if (n->type == AST_NODE_PROC)
	{
		n->data.proc.has_redgrouptoks = val;
	}
}

void	p_set_do_redir_expansion(t_ast_node *n, bool val)
{
	if (!n)
		return ;
	if (n->type == AST_NODE_CMD)
		n->data.cmd.do_redir_expansion = val;
	else if (n->type == AST_NODE_PROC)
		n->data.proc.do_redir_expansion = val;
}

void	p_set_do_redir_globbing(t_ast_node *n, bool val)
{
	if (!n)
		return ;
	if (n->type == AST_NODE_CMD)
		n->data.cmd.do_redir_globbing = val;
	else if (n->type == AST_NODE_PROC)
		n->data.proc.do_redir_globbing = val;
}
