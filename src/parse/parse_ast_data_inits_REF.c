#include "parse_int.h"

#define EMSG_REDIR_DATA_MALLOC "Memory allocation failed for redirection data\n"

t_redir_data	*init_redir(t_mem_mgr *m, t_ast_node *target,
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
	red->heredoc_body = NULL;
	red->symbol = NULL;
	red->filename = NULL;
	red->global_state = NULL;
	red->lst_glob = NULL;
	return (red);
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
		arg->option = is_option(tok);
		arg->do_globbing = tok_get_globbing(tok);
		arg->do_expansion = tok_get_expansion(tok);
		arg->in_dquotes = tok_get_dquotes(tok);
		arg->is_grouparg = tok_isgrouptoken(tok);
		arg->tmp = NULL;
		arg->lst_tokens = ft_lstcopy_tmp(m, tok_get_tlist(tok), copy_token,
				destroy_token);
		arg->global_state = p->global_state;
		if (false == arg->in_dquotes)
			cmd_node->data.cmd.do_wordsplit = true;
		if (true == arg->do_expansion)
			cmd_node->data.cmd.do_expansion = true;
		if (true == arg->do_globbing)
			cmd_node->data.cmd.do_globbing = true;
		if (true == arg->is_grouparg)
			cmd_node->data.cmd.has_grouptoks = true;
	}
	return (arg);
}

/* t_arg_data llist copy constructor using a llist of char*.
 * Returns new void *content for llist construction/duplication.
 * Copies content.
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
