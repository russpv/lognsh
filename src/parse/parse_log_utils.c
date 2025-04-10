/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_log_utils.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rpeavey <rpeavey@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/10 16:33:55 by rpeavey           #+#    #+#             */
/*   Updated: 2025/04/10 16:33:56 by rpeavey          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parse_int.h"

t_ast_node	*init_log(t_mem_mgr *m)
{
	t_ast_node	*log_node;

	log_node = m->f(&m->list, sizeof(t_ast_node));
	if (log_node)
	{
		log_node->type = AST_NODE_LOG;
		log_node->data.log.cmds = NULL;
		log_node->data.log.cmdc = 0;
		log_node->data.log.operators = NULL;
	}
	return (log_node);
}

/* Returns cmp if it is a lower priority operator than
 * that represented by ref.
 */
t_tok	*which_lower_priority(t_ast_node *ref, t_tok *cmp)
{
	if (!ref | !cmp)
		return (NULL);
	if (is_close_paren(cmp) || tok_get_type(cmp) == TOK_EOF)
		return (cmp);
	if (is_group_op_token(cmp))
	{
		if (ref->type == AST_NODE_PIPELINE && is_log_token(cmp))
			return (cmp);
	}
	return (NULL);
}
