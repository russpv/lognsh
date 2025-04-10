/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_ast_frees.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rpeavey <rpeavey@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/10 16:33:26 by rpeavey           #+#    #+#             */
/*   Updated: 2025/04/10 17:51:28 by rpeavey          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parse_int.h"

void	destroy_cmd_node(t_mem_mgr *mgr, void **n)
{
	t_ast_node	*node;

	node = (t_ast_node *)(*n);
	dvprint(_MOD_ ":  destroy_cmd_node...\n");
	if (NULL == node)
		return ;
	if (node->type != AST_NODE_CMD)
		return ;
	if (node->data.cmd.name)
		mgr->dealloc(&mgr->list, node->data.cmd.name);
	if (node->data.cmd.args)
		ft_lstclear_tmp(mgr, &node->data.cmd.args, destroy_arg);
	if (node->data.cmd.redirs)
		ft_lstclear_tmp(mgr, &node->data.cmd.redirs, destroy_redir);
	mgr->dealloc(&mgr->list, node);
	dvprint(_MOD_ ":  destroy_cmd_node...DONE\n");
	*n = NULL;
}

void	destroy_proc_node(t_mem_mgr *mgr, void **n)
{
	t_ast_node	*node;

	node = (t_ast_node *)(*n);
	dvprint(_MOD_ ":  destroy_proc_node...\n");
	if (NULL == node)
		return ;
	if (AST_NODE_PROC != node->type)
		return ;
	if (node->data.proc.redirs)
		ft_lstclear_tmp(mgr, &node->data.proc.redirs, destroy_redir);
	if (node->data.proc.cmds)
		ft_lstclear_tmp(mgr, &node->data.proc.cmds, destroy_ast_node);
	mgr->dealloc(&mgr->list, node);
	dvprint(_MOD_ ":  destroy_proc_node...DONE\n");
	*n = NULL;
}

// destroy t_list cmds, char **operators
void	destroy_log_node(t_mem_mgr *mgr, void **n)
{
	t_ast_node	*node;

	node = (t_ast_node *)(*n);
	dvprint(_MOD_ ":  destroy_log_node...\n");
	if (NULL == node)
		return ;
	if (AST_NODE_LOG != node->type)
		return ;
	if (node->data.log.operators)
		ft_lstclear_str_tmp(mgr, &node->data.log.operators);
	if (node->data.log.cmds)
		ft_lstclear_tmp(mgr, &node->data.log.cmds, destroy_ast_node);
	mgr->dealloc(&mgr->list, node);
	dvprint(_MOD_ ":  destroy_log_node...DONE\n");
	*n = NULL;
}

void	destroy_pipe_node(t_mem_mgr *mgr, void **n)
{
	t_ast_node	*node;

	node = (t_ast_node *)(*n);
	dvprint(_MOD_ ":  destroy_pipe_node...\n");
	if (NULL == node)
		return ;
	if (AST_NODE_PIPELINE != node->type)
		return ;
	if (node->data.pipe.cmds)
		ft_lstclear_tmp(mgr, &node->data.pipe.cmds, destroy_ast_node);
	mgr->dealloc(&mgr->list, node);
	dvprint(_MOD_ ":  destroy_pipe_node...DONE\n");
	*n = NULL;
}

/* Switch for freeing various t_ast_node types
 * For use in llist destroy
 */
void	destroy_ast_node(t_mem_mgr *mgr, void **node)
{
	t_ast_node	*ast;

	ast = (t_ast_node *)(*node);
	dvprint(_MOD_ ":  destroy_ast...\n");
	if (NULL == ast)
		return ;
	if (NONE == ast->type)
		return ;
	else if (AST_NODE_PROC == ast->type)
		destroy_proc_node(mgr, node);
	else if (AST_NODE_CMD == ast->type)
		destroy_cmd_node(mgr, node);
	else if (AST_NODE_PIPELINE == ast->type)
		destroy_pipe_node(mgr, node);
	else
		destroy_log_node(mgr, node);
	dvprint(_MOD_ ":  destroy_ast...DONE\n");
	*node = NULL;
}
