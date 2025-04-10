/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   command.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rpeavey <rpeavey@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/10 16:29:23 by rpeavey           #+#    #+#             */
/*   Updated: 2025/04/10 16:29:24 by rpeavey          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "command_int.h"

#define LMSG_IN  "%s: ######## %s ########\n"
#define DMSG_CEXEC_NOTPROC  "%s: node not a proc...\n"
#define DMSG_CEXEC_NOTCMD  "%s: node not a cmd...\n"
#define DMSG_CEXEC_NOTPIP  "%s: node not a pipe...\n"
#define EMSG  "ERR unknown node...\n"

/* Determines which parser to call given current token
 * note: cmd_exec_simple is atomic and does not recurse
 */
int	cmd_execute_full(t_state *s, t_ast_node *a)
{
	lgprint(LMSG_IN, _MOD_, __FUNCTION__);
	if (AST_NODE_PROC == p_get_type(a))
		return (cmd_exec_proc(s, a));
	dvprint(DMSG_CEXEC_NOTPROC, _MOD_);
	if (AST_NODE_CMD == p_get_type(a))
		return (cmd_exec_simple(s, a));
	dvprint(DMSG_CEXEC_NOTCMD, _MOD_);
	if (AST_NODE_PIPELINE == p_get_type(a))
		return (cmd_exec_pipe(s, a));
	dvprint(DMSG_CEXEC_NOTPIP, _MOD_);
	if (AST_NODE_LOG == p_get_type(a))
		return (cmd_exec_log(s, a));
	err(EMSG);
	return (ERR_SYNTAX);
}

/* CMD EXECUTE
 * Recursively executes the AST from Parser
 * Sets current_cmd in State then calls
 * top 'switchboard' func
 * Returns last exit_code from input
 *
 * Setting cmd in State allows multiple accesses
 * for expansions, globbing, etc.
 *
 * The Command ADT stack tracks current AST depth
 * When a sub-method is called, the current node is
 * pushed. And when that sub-method returns, it is
 * popped. Why?
 */
int	cmd_execute(t_state *s, t_ast_node *a)
{
	if (NULL == init_cmd(s, a))
		exit_clean(&get_mem(s)->list, ENOMEM, __FUNCTION__, EMSG_MALLOC);
	print_ast(a, 10);
	return (cmd_execute_full(s, a));
}
