#include "parse_int.h"

/* Must deep copy token strings to decouple token-list/ast */
static void	_init_arg_data(t_arg_data *arg, t_tok *tok)
{
	arg->raw = ft_strdup(tok_get_raw(tok));
	if (!arg->raw)
		err("_init_arg_data malloc err\n");
	arg->option = is_option(tok);
	arg->do_globbing = tok_get_globbing(tok);
	arg->do_expansion = tok_get_expansion(tok);
	arg->in_dquotes = tok_get_dquotes(tok);
	arg->tmp = NULL;
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

/* If flag set, looks for expansion values in ENV and replaces 
 * t_arg_data.raw string 
 * Applied to t_ast_node.cmd's t_list*
 */
void	p_do_expansion(void *c)
{
	char buf[1024];

	ft_memset(buf, 0, 1024);
	t_arg_data *content = (t_arg_data *)c;
	debug_print("p_do_expansion got: %s\n", content->raw);
	if (content->do_expansion)
	{
		ft_memcpy(buf, content->raw + 1, ft_strnlen(content->raw, 1024) - 1);
		const char *value = getenv(buf);
		debug_print("p_do_expansion found: %s\n", value);
		if (value)
		{
			free(content->raw);
			content->raw = ft_strdup(value);
			if (!content->raw)
				err("p_do_expansion Malloc\n");
		}
	}
}

bool	is_expansion(t_tok *tok)
{
	const enum e_tok_type	type = tok_get_type(tok);

	return (type == TOK_EXIT_STATUS || type == TOK_ENV_VAR);
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
