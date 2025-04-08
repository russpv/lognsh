#include "parse_int.h"


bool	p_get_has_redgrouptoks(t_ast_node *n)
{
	if (!n)
		return (false);
	if (n->type == AST_NODE_CMD)
	 	return (n->data.cmd.do_redir_expansion);
	if (n->type == AST_NODE_PROC)
		return (n->data.proc.do_redir_expansion);
	return (false);
}

bool	p_get_do_redir_expansion(t_ast_node *n)
{
	if (!n)
		return (false);
	if (n->type == AST_NODE_CMD)
	 	return (n->data.cmd.do_redir_expansion);
	if (n->type == AST_NODE_PROC)
		return (n->data.proc.do_redir_expansion);
	return (false);
}


bool	p_get_do_redir_globbing(t_ast_node *n)
{
	if (!n)
		return (false);
	if (n->type == AST_NODE_CMD)
	 	return (n->data.cmd.do_redir_expansion);
	if (n->type == AST_NODE_PROC)
		return (n->data.proc.do_redir_expansion);
	return (false);
}
