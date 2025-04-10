/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_do_arg.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rpeavey <rpeavey@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/10 16:33:31 by rpeavey           #+#    #+#             */
/*   Updated: 2025/04/10 16:33:32 by rpeavey          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parse_int.h"

#define EMSG_PATH_MALLOC "Allocation for path value failed.\n"
#define EMSG_NULL_EXPAN "Null expansion variable.\n"

// Processes any group args first, then normal args
static int	_helper(t_state *s, t_ast_node *a, t_list **argl)
{
	int	res;

	res = 0;
	if (a->data.cmd.has_arggrouptoks)
		res = ft_lstiter_state_ins_rwd_mem(s, argl, p_do_grparg_processing);
	if (a->data.cmd.do_expansion)
		res = lstiter_state(s, *argl, p_do_arg_expansion);
	if (a->data.cmd.do_expansion && a->data.cmd.do_wordsplit)
		ft_lstiter_ins_rwd_tmp(get_mem(s), argl, p_do_wordsplits);
	if (a->data.cmd.do_globbing)
		ft_lstiter_ins_rwd_tmp(get_mem(s), argl, p_do_globbing_args);
	return (res);
}

/* PROCESS ARGS
 *
 * Does expansions: Shell parameters, word splitting,
 * filename expansions (glob*) as needed.
 * Then converts to array and returns that array via args ptr.
 * Note: we traverse llists backwards to avoid inserted nodes
 * Note: procs return early, since no args.
 */
int	p_process_args(t_state *s, t_ast_node *a, char ***args)
{
	t_list	**argl;
	int		res;

	res = 0;
	if (a->type == AST_NODE_PROC)
		return (0);
	if (a->type != AST_NODE_CMD)
		return (err("Invalid node type"), ERR_INVALID_CMD_TYPE);
	argl = p_get_args(a);
	if (*argl)
	{
		res = _helper(s, a, argl);
		if (0 != res)
			return (res);
		a->data.cmd.argc = ft_lstsize(*argl);
		res = lstiter_state_rwd_trim(s, argl, p_is_arg_null, destroy_arg);
		if (res < 0)
			a->data.cmd.argc += res;
		dprint_arglist(*argl);
		*args = list_to_array(get_mem(s), *argl, a->data.cmd.argc);
		if (NULL == *args)
			return (ERR_MEM);
	}
	return (0);
}
