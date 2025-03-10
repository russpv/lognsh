#include "parse_int.h"

void	destroy_cmd_node(t_state *s, void *n)
{
	t_ast_node	*node;

	node = (t_ast_node *)n;
	debug_print(_MOD_ ":  destroy_cmd_node...\n");
	if (NULL == node)
		return ;
	if (node->type != AST_NODE_CMD)
		return ;
	if (node->data.cmd.name)
		free(node->data.cmd.name);
	if (node->data.cmd.args)
		ft_lstclear_tmp(s, &node->data.cmd.args, destroy_arg);
	if (node->data.cmd.redirs)
		ft_lstclear(&node->data.cmd.redirs, destroy_redir);
	free(node);
	debug_print(_MOD_ ":  destroy_cmd_node...DONE\n");
}

void	destroy_proc_node(t_state *s, void *n)
{
	t_ast_node	*node;

	node = (t_ast_node *)n;
	debug_print(_MOD_ ":  destroy_proc_node...\n");
	if (NULL == node)
		return ;
	if (AST_NODE_PROC != node->type)
		return ;
	if (node->data.proc.redirs)
		ft_lstclear(&node->data.proc.redirs, destroy_redir);
	if (node->data.proc.cmds)
		ft_lstclear_tmp(s, &node->data.proc.cmds, destroy_ast_node);
	free(node);
	debug_print(_MOD_ ":  destroy_proc_node...DONE\n");
}

// destroy t_list cmds, char **operators
void	destroy_log_node(t_state *s, void *n)
{
	t_ast_node	*node;

	node = (t_ast_node *)n;
	debug_print(_MOD_ ":  destroy_log_node...\n");
	if (NULL == node)
		return ;
	if (AST_NODE_LOG != node->type)
		return ;
	if (node->data.log.operators)
		ft_lstclear(&node->data.log.operators, free);
	if (node->data.log.cmds)
		ft_lstclear_tmp(s, &node->data.log.cmds, destroy_ast_node);
	free(node);
	debug_print(_MOD_ ":  destroy_log_node...DONE\n");
}

void	destroy_pipe_node(t_state *s, void *n)
{
	t_ast_node	*node;

	node = (t_ast_node *)n;
	debug_print(_MOD_ ":  destroy_pipe_node...\n");
	if (NULL == node)
		return ;
	if (AST_NODE_PIPELINE != node->type)
		return ;
	if (node->data.pipe.cmds)
		ft_lstclear_tmp(s, &node->data.pipe.cmds, destroy_ast_node);
	free(node);
	debug_print(_MOD_ ":  destroy_pipe_node...DONE\n");
}

/* Switch for freeing various t_ast_node types
 * For use in llist destroy
 */
void	destroy_ast_node(t_state *s, void *node)
{
	t_ast_node	*ast;

	ast = (t_ast_node *)node;
	debug_print(_MOD_ ":  destroy_ast...\n");
	if (NULL == ast)
		return ;
	if (AST_NODE_PROC == ast->type)
		destroy_proc_node(s, ast);
	else if (AST_NODE_CMD == ast->type)
		destroy_cmd_node(s, ast);
	else if (AST_NODE_PIPELINE == ast->type)
		destroy_pipe_node(s, ast);
	else
		destroy_log_node(s, ast);
	debug_print(_MOD_ ":  destroy_ast...DONE\n");
}
