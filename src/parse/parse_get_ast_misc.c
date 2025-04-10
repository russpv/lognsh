/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_get_ast_misc.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rpeavey <rpeavey@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/10 16:33:49 by rpeavey           #+#    #+#             */
/*   Updated: 2025/04/10 16:33:50 by rpeavey          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parse_int.h"

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
