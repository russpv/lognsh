/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cmd_simple.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rpeavey <rpeavey@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/10 16:29:14 by rpeavey           #+#    #+#             */
/*   Updated: 2025/04/12 23:02:51 by rpeavey          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "command_int.h"

static int	_handle_no_command(t_ast_node *a)
{
	if (p_get_type(a) != AST_NODE_CMD)
		return (0);
	if (NULL == p_get_cmd(a))
		if (false == p_get_expansion(a) && false == p_get_grouptok(a))
			return (NO_CMD);
	return (0);
}

/* Processes args and redirs. Stores several command parameters.
 * Puts node a into cmd c
 * Used by Proc and Simple.
 * Note: if any arguments are null, like a null heredoc
 */
int	proc_args_redirs(t_state *s, t_ast_node *a, t_cmd *c)
{
	char	**args;
	int		exit_code;
	int		res;

	c->curr_node = a;
	args = NULL;
	res = p_process_args(s, a, &args);
	if (0 != res)
		return (res);
	exit_code = _handle_no_command(a);
	if (NO_CMD == exit_code)
		return (exit_code);
	if (p_get_type(a) == AST_NODE_CMD)
		c_argstoargv(s, c, a, args);
	c->redc = p_get_redc(a);
	if (0 != p_do_redir_processing(s, a))
		return (ERR_REDIR);
	c->redirs = p_get_redirs(a);
	dprintdiv("\n--- Redirection Processing Done ---\n\n");
	return (exit_code);
}

/* CMD EXEC SIMPLE
 *
 * For an AST COMMAND node only
 * Checks for no command, builtin, then PATH
 * Prints diagnostic info.
 * Runs expansion then globbing and stores
 * 	results into t_cmd not t_ast_node.
 * Throws ambiguous redirect error (1) prior to exec'g.
 * Stores argv and argc into s->curr_cmd.
 * Silently returns if non-expansion string
 */
int	cmd_exec_simple(t_state *s, t_ast_node *a)
{
	int				exit_code;
	t_cmd			*c;
	t_builtin_fn	bi;
	t_cmd_fns		*cf;

	c = get_cmd(s);
	lgprint("%s: \t### %s ###\n", _MOD_, __FUNCTION__);
	if (!c || !a)
		return (ERR_ARGS);
	if (p_get_type(a) != AST_NODE_CMD)
		return (ERR_INVALID_CMD_TYPE);
	exit_code = proc_args_redirs(s, a, c);
	if (ERR_REDIR == exit_code || NO_CMD == exit_code)
		return (exit_code);
	log_command_info((t_cmd *)c, a);
	cf = init_cmd_fns(s);
	bi = get_builtin(p_get_cmd(a));
	if (bi)
		exit_code = exec_bi_call(s, bi, cf);
	else
		exit_code = run_cmd(s, a);
	lgprint("%s: \t%s exit code: %d.\n", _MOD_, __FUNCTION__, exit_code);
	destroy_cmd_fns(get_mem(s), cf);
	return (exit_code);
}
