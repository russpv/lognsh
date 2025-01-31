#include "parse_int.h"

/* Must deep copy token strings to decouple token-list/ast */
static void	_init_arg_data(t_parser *p, t_ast_node *cmd_node, t_arg_data *arg, t_tok *tok)
{
	arg->raw = ft_strdup(tok_get_raw(tok));
	if (!arg->raw)
		err("_init_arg_data malloc err\n");
	arg->option = is_option(tok);
	arg->do_globbing = tok_get_globbing(tok);
	arg->do_expansion = tok_get_expansion(tok);
	arg->in_dquotes = tok_get_dquotes(tok);
	arg->tmp = NULL;
	arg->global_state = p->global_state;
	if (true == arg->do_expansion)
		cmd_node->data.cmd.do_expansion = true;
	if (true == arg->do_globbing)
		cmd_node->data.cmd.do_globbing = true;
}

/* Consumes arg tokens and adds them to command node linked list
 * Returns NULL if syntax error
 */
static t_list	*_parse_args(t_parser *p, t_ast_node *cmd_node)
{
	t_arg_data	*arg;
	t_list		*new;

	while (!is_at_end(p) && is_arg_token(peek(p)))
	{
		arg = malloc(sizeof(t_arg_data));
		if (!arg)
		{
			err("Memory allocation error for args\n");
			return (NULL);
		}
		_init_arg_data(p, cmd_node, arg, advance(p));
		new = ft_lstnew(arg);
		if (!new)
		{
			err("Memory allocation error adding arg node\n");
			free(arg);
			return (NULL);
		}
		ft_lstadd_back(&cmd_node->data.cmd.args, new);
		cmd_node->data.cmd.argc++;
	}
	return (cmd_node->data.cmd.args);
}

static t_ast_node	*_process_cmd(t_parser *p, t_ast_node *cmd_node)
{	
	if (0 != process_redir(p, cmd_node))
		return (NULL);
	if (!is_cmd_token(peek(p)))
	{
		free(cmd_node);
		err("Expected a command token, but none found\n");
		return (NULL);
	}
	if (!is_expansion(peek(p)))
		cmd_node->data.cmd.name = tok_get_raw(advance(p));
	else
		cmd_node->data.cmd.name = NULL;
	_parse_args(p, cmd_node);
	if (0 != process_redir(p, cmd_node))
		return (NULL);
	return (cmd_node);
}

static void	_init_cmd_data(t_ast_node *cmd_node)
{
	cmd_node->type = AST_NODE_CMD;
	cmd_node->data.cmd.args = NULL;
	cmd_node->data.cmd.redirs = NULL;
	cmd_node->data.cmd.redc = 0;
	cmd_node->data.cmd.argc = 0;
}

t_ast_node	*parse_cmd(t_parser *p)
{
	t_ast_node	*cmd_node;

	st_push(p->st, AST_NODE_CMD);
	cmd_node = malloc(sizeof(t_ast_node));
	if (cmd_node)
	{
		debug_print("cmd tok: %s\n", tok_get_raw(peek(p)));
		_init_cmd_data(cmd_node);
		if (!_process_cmd(p, cmd_node))
			return (NULL);
	}
	else
	{
		err("Memory allocation failed for command node\n");
		return (NULL);
	}
	p->last_node = cmd_node;
	st_pop(p->st);
	return (cmd_node);
}
