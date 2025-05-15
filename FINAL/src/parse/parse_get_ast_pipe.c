/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_get_ast_pipe.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rpeavey <rpeavey@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/10 16:33:50 by rpeavey           #+#    #+#             */
/*   Updated: 2025/04/10 16:33:51 by rpeavey          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parse_int.h"

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
