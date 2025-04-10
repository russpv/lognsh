/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_get_ast_proc.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rpeavey <rpeavey@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/10 16:33:51 by rpeavey           #+#    #+#             */
/*   Updated: 2025/04/10 16:33:52 by rpeavey          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parse_int.h"

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
