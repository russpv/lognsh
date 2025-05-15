/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_ast_data_inits.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rpeavey <rpeavey@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/10 16:33:25 by rpeavey           #+#    #+#             */
/*   Updated: 2025/04/12 17:45:57 by rpeavey          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parse_int.h"

#define EMSG_REDIR_DATA_MALLOC "Memory allocation failed for redirection data\n"

// Called on redir tokens, which only have the symbol info.
t_redir_data	*init_redir(t_mem_mgr *m, t_parser *p, t_ast_node *target,
		enum e_tok_type type)
{
	t_redir_data	*red;

	if (!target)
		return (NULL);
	red = m->f(&m->list, sizeof(t_redir_data));
	if (!red)
		exit_clean(&m->list, ENOMEM, __FUNCTION__, EMSG_REDIR_DATA_MALLOC);
	red->type = type;
	red->target_ptr = target;
	red->do_globbing = false;
	red->do_expansion = false;
	red->in_dquotes = false;
	red->is_groupred = false;
	red->heredoc_body = NULL;
	red->symbol = NULL;
	red->filename = NULL;
	red->global_state = p->global_state;
	red->lst_glob = NULL;
	red->lst_tokens = NULL;
	return (red);
}

static void	_init_arg_helper(t_arg_data *arg, t_parser *p, t_tok *tok)
{
	arg->global_state = p->global_state;
	arg->tmp = NULL;
	arg->option = is_option(tok);
	arg->do_globbing = tok_get_globbing(tok);
	arg->do_expansion = tok_get_expansion(tok);
	arg->in_dquotes = tok_get_dquotes(tok);
	arg->is_grouparg = tok_isgrouptoken(tok);
}

/* Must deep copy token strings to decouple token-list/ast.
 */
t_arg_data	*init_arg(t_mem_mgr *m, t_parser *p, t_ast_node *cmd_node,
		t_tok *tok)
{
	t_arg_data	*arg;

	if (!p || !cmd_node || !tok)
		return (NULL);
	arg = m->f(&m->list, sizeof(struct s_arg));
	if (arg)
	{
		if (tok_get_raw(tok))
		{
			arg->raw = ft_strdup_tmp(m, tok_get_raw(tok));
			if (!arg->raw)
				exit_clean(&m->list, ENOMEM, __FUNCTION__, EMSG_MALLOC);
		}
		else
			arg->raw = NULL;
		_init_arg_helper(arg, p, tok);
		arg->lst_tokens = ft_lstcopy_tmp(m, tok_get_tlist(tok), copy_token,
				destroy_token);
		if (false == arg->in_dquotes)
			cmd_node->data.cmd.do_wordsplit = true;
		cmd_node->data.cmd.do_expansion |= arg->do_expansion;
		cmd_node->data.cmd.do_globbing |= arg->do_globbing;
		cmd_node->data.cmd.has_arggrouptoks |= arg->is_grouparg;
	}
	return (arg);
}

/* t_arg_data llist copy constructor using a llist of char*.
 * Returns new void *content for llist construction/duplication.
 * Post processing, t_arg_data member values are no longer useful
 * Copies content. Ensures globbing pass is done.
 */
void	*create_arg_data_node_deep(t_mem_mgr *mgr, const void *content)
{
	t_arg_data	*arg_data;
	char		*raw;

	raw = (char *)content;
	arg_data = mgr->f(&mgr->list, sizeof(t_arg_data));
	if (arg_data)
	{
		arg_data->raw = ft_strdup_tmp(mgr, raw);
		if (!arg_data->raw)
			exit_clean(&mgr->list, ENOMEM, __FUNCTION__, EMSG_MALLOC);
		arg_data->do_expansion = false;
		arg_data->do_globbing = false;
		arg_data->global_state = NULL;
		arg_data->in_dquotes = false;
		arg_data->option = false;
		arg_data->tmp = NULL;
		arg_data->is_grouparg = false;
		arg_data->lst_tokens = NULL;
	}
	return ((void *)arg_data);
}

/* t_arg_data llist copy constructor using a llist of char*.
 * Returns new void *content for llist construction/duplication.
 * Only the target filename is needed.
 * Inits t_redir_data dummy values.
 */
void	*create_redir_data_node_deep(t_mem_mgr *mgr, const void *content)
{
	t_redir_data	*red_data;
	char			*fn;

	fn = (char *)content;
	red_data = mgr->f(&mgr->list, sizeof(t_redir_data));
	if (red_data)
	{
		red_data->filename = ft_strdup_tmp(mgr, fn);
		if (!red_data->filename)
			exit_clean(&mgr->list, ENOMEM, __FUNCTION__, EMSG_MALLOC);
		red_data->do_expansion = false;
		red_data->do_globbing = false;
		red_data->in_dquotes = false;
		red_data->is_groupred = false;
		red_data->lst_tokens = NULL;
		red_data->lst_glob = NULL;
		red_data->global_state = NULL;
		red_data->heredoc_body = NULL;
		red_data->symbol = NULL;
		red_data->target_ptr = NULL;
		red_data->type = 0;
	}
	return ((void *)red_data);
}
