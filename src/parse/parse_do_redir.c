/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_do_redir.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rpeavey <rpeavey@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/10 16:33:43 by rpeavey           #+#    #+#             */
/*   Updated: 2025/04/12 23:56:18 by rpeavey          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parse_int.h"

#define EMSG_REDIR_HANDLER "Redirection handler issue\n"
#define EMSG_REDIR_NULLNODE "ERR no node given\n"
#define DM_R_G "%s: p_do_redir_processing got: type:%d glob_%d exp_%d grp_%d\n"

/* Executes redirection of t_redir_data llist
 * Accepts a t_redir_data.
 * Assumes a non-null set of redirections exists.
 * Note: parser does not node-rize TOK_HEREDOC
 */
static int	_p_do_redirection(void *content)
{
	const t_redir_data	*redir = (t_redir_data *)content;
	const t_redir_fn	handlers[] = {[TOK_REDIRECT_IN] = handle_redirect_in,
	[TOK_REDIRECT_OUT] = handle_redirect_out,
	[TOK_REDIRECT_APPEND] = handle_redirect_append,
	[TOK_HEREDOC_WORD] = handle_heredoc};

	if (NULL == content)
		return (ERR_ARGS);
	if (!redir->type)
		return (dprint("%s: %s: got null\n", _MOD_, __FUNCTION__), ERR_ARGS);
	dprint("%s: %s: iterating...\n", _MOD_, __FUNCTION__);
	if (handlers[redir->type])
	{
		if (0 != handlers[redir->type](redir))
			return (err(EMSG_REDIR_HANDLER), ERR_REDIR);
		if (EACCES == errno)
			set_exit_status(redir->global_state, ERR_GENERAL);
	}
	else
		return (err(EMSG_REDIR_HANDLER), ERR_REDIR);
	return (0);
}

/* Executes redirections of a node in order.
 * If no redirections to do, returns ...
 * Expansions and globbing are to have been done earlier.
 * If error during redirection due to...
 * 	- multiple filenames, aborts command only
 *  - nonexistent input file, aborts command only
 *  - nonexistent output file, creates file and proceeds (no error)
 *  - output is a dir, aborts command only
 */
int	p_do_redirections(t_ast_node *a)
{	
	if (!a)
		return (err(EMSG_REDIR_NULLNODE), ERR_ARGS);
	if (a->type != AST_NODE_CMD && a->type != AST_NODE_PROC)
		return (err("Redirs: invalid cmd type\n"), EINVAL);
	if (false == node_has_redirects(a))
		return (0);
	dprint("%s: %s: doing redirections...\n", _MOD_, __FUNCTION__);
	if (a->type == AST_NODE_CMD)
		if (0 != ft_lstiter(a->data.cmd.redirs, _p_do_redirection))
			return (ERR_REDIR);
	if (a->type == AST_NODE_PROC)
		if (0 != ft_lstiter(a->data.proc.redirs, _p_do_redirection))
			return (ERR_REDIR);
	return (0);
}

static void	_dprint(t_ast_node *a)
{
	dprint(DM_R_G, _MOD_, p_get_type(a),
		p_get_do_redir_glob(a), p_get_do_redir_exp(a),
		p_get_has_redgrptok(a));
}

static int	_do_normal(t_state *s, t_ast_node *a, t_list **redirs)
{
	char	*orig_filen;
	int		res;

	res = 0;
	orig_filen = NULL;
	if (true == p_get_do_redir_exp(a))
		res = lstiter_state(s, *redirs, p_do_red_expansion);
	if (0 != res)
		return (res);
	if (true == p_get_do_redir_glob(a))
		orig_filen = ft_lstiterstr_mem(get_mem(s), *redirs,
				p_do_globbing_redirs);
	if (NULL != orig_filen)
	{
		print_ambig_redir(((t_redir_data *)orig_filen)->filename);
		return (ERR_AMBIGUOUS_REDIR);
	}
	return (0);
}

/* Modifies redirection list in case of expansions.
 */
int	p_do_redir_processing(t_state *s, t_ast_node *a)
{
	t_list **const	redirs = p_get_redirs_ptr(a);
	int				res;

	if (a->type != AST_NODE_CMD && a->type != AST_NODE_PROC)
		return (err("Invalid node type\n"), ERR_ARGS);
	if (redirs)
	{
		_dprint(a);
		if (true == p_get_has_redgrptok(a))
		{
			res = ft_lstiter_state_ins_rwd_mem(s, redirs,
					p_do_grpred_processing);
			if (0 != res)
				return (res);
		}
		else
		{
			res = _do_normal(s, a, redirs);
			if (0 != res)
				return (res);
		}
		dprint("%s: %s: done.\n", _MOD_, __FUNCTION__);
	}
	return (0);
}
