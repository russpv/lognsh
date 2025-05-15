/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_get_ast_cmd.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rpeavey <rpeavey@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/10 16:33:47 by rpeavey           #+#    #+#             */
/*   Updated: 2025/04/10 16:33:48 by rpeavey          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parse_int.h"

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
