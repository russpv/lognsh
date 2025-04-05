/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   command.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dayeo <dayeo@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/05 09:27:36 by dayeo             #+#    #+#             */
/*   Updated: 2025/04/05 11:40:41 by dayeo            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "command_int.h"

void	destroy_cmd(t_mem_mgr *m, void **c)
{
	t_cmd		*cmd;

	cmd = (t_cmd *)(*c);
	if (cmd->st)
		st_int_destroy(m, cmd->st);
	if (cmd->redirs)
		ft_lstclear_tmp(m, &cmd->redirs, destroy_redir);
	if (cmd->fildes)
		ft_freearr_mem(&m->list, m->dealloc, (void **)cmd->fildes, -1);
	if (cmd->argv)
		ft_freearr_mem(&m->list, m->dealloc, (void **)cmd->argv, -1);
	m->dealloc(&m->list, cmd);
	*c = NULL;
}

t_cmd	*init_cmd(t_state *s, t_ast_node *a)
{
	t_cmd	*c;

	c = get_mem(s)->f(&get_mem(s)->list, sizeof(struct s_cmd));
	if (c)
	{
		c->fildes = NULL;
		c->st = st_int_create(get_mem(s));
		c->argv = NULL;
		c->argc = 0;
		c->redirs = NULL;
		c->redc = 0;
		c->fullpath = NULL;
		c->curr_node = a;
		c->saved_stderr = -1;
		c->saved_stdin = -1;
		c->saved_stdout = -1;
		c->curr_cmdc = -1;
		c->argvc = 0;
		s_free_tmp(s);
		set_command(s, c);
		register_command_destroy(s, destroy_cmd);
	}
	return (c);
}

/* Determines which parser to call given current token
 * note: cmd_exec_simple is atomic and does not recurse
 */
int	cmd_execute_full(t_state *s, t_ast_node *a)
{
	if (AST_NODE_PROC == p_get_type(a))
		return (cmd_exec_proc(s, a));
	if (AST_NODE_CMD == p_get_type(a))
		return (cmd_exec_simple(s, a));
	if (AST_NODE_PIPELINE == p_get_type(a))
		return (cmd_exec_pipe(s, a));
	if (AST_NODE_LOG == p_get_type(a))
		return (cmd_exec_log(s, a));
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
		return (ERR_MEM);
	print_ast(a, 10);
	return (cmd_execute_full(s, a));
}
