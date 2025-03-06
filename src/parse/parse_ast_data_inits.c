#include "parse_int.h"

#define EMSG_REDIR_DATA_MALLOC "Memory allocation failed for redirection data\n"

t_redir_data	*init_redir(t_ast_node *target, enum e_tok_type type)
{
	t_redir_data	*red;

	if (!target)
		return (NULL);
	red = malloc(sizeof(t_redir_data));
	if (!red)
	{
		err(EMSG_REDIR_DATA_MALLOC);
		return (NULL);
	}
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
t_arg_data	*init_arg(t_parser *p, t_ast_node *cmd_node, t_tok *tok)
{
	t_arg_data	*arg;

	if (!p || !cmd_node || !tok)
		return (NULL);
	arg = malloc(sizeof(struct s_arg));
	if (arg)
	{
		if (tok_get_raw(tok))
		{
			arg->raw = ft_strdup(tok_get_raw(tok));
			if (!arg->raw)
				return (err(EMSG_MALLOC), free(arg), NULL);
		}
		else
			arg->raw = NULL;
		arg->option = is_option(tok);
		arg->do_globbing = tok_get_globbing(tok);
		arg->do_expansion = tok_get_expansion(tok);
		arg->in_dquotes = tok_get_dquotes(tok);
		arg->is_grouparg = tok_isgrouptoken(tok);
		arg->tmp = NULL;
		arg->lst_tokens = ft_lstcopy(tok_get_tlist(tok), copy_token, destroy_token);
		arg->global_state = p->global_state;
		if (true == arg->do_expansion)
			cmd_node->data.cmd.do_expansion = true;
		if (true == arg->do_globbing)
			cmd_node->data.cmd.do_globbing = true;
		if (true == arg->is_grouparg)
			cmd_node->data.cmd.has_grouptoks = true;
	}
	return (arg);
}
