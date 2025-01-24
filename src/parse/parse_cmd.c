#include "parse_int.h"

static void	_init_arg_data(t_arg_data *arg, t_tok *tok)
{
	arg->raw = tok_get_raw(tok);
	arg->option = is_option(tok);
	arg->do_globbing = tok_get_globbing(tok);
	arg->do_expansion = tok_get_expansion(tok);
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
		_init_arg_data(arg, advance(p));
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
	fprintf(stderr, "proc'g cmd %s\n", tok_get_raw(peek(p)));
	if (is_redir_token(peek(p)))
	{
		fprintf(stderr, "proc'g redir %s\n", tok_get_raw(peek(p)));
		if (!parse_redir(p, cmd_node))
		{
			free(cmd_node);
			err("Failed to parse redirection\n");
			return (NULL);
		}
	}
	if (!is_cmd_token(peek(p)))
	{
		free(cmd_node);
		err("Expected a command token, but none found\n");
		fprintf(stderr, "tok: %s", tok_get_raw(peek(p)));
		return (NULL);
	}
	cmd_node->data.cmd.name = tok_get_raw(advance(p));
	_parse_args(p, cmd_node);
	if (is_redir_token(peek(p)))
	{
		if (!parse_redir(p, cmd_node))
		{
			free(cmd_node);
			err("Failed to parse redirection\n");
			return (NULL);
		}
	}
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
		fprintf(stderr, "tok: %s", tok_get_raw(peek(p)));
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
