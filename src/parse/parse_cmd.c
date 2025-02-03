#include "parse_int.h"

/* t_arg_data llist copy constructor using a llist of char*.
 * Returns new void *content for llist construction/duplication.
 */
void *create_arg_data_node(void *content)
{
	content = (char *)content;
	t_arg_data *arg_data = malloc(sizeof(t_arg_data)); 
	if (arg_data)
	{
		arg_data->do_expansion = false;
		arg_data->do_globbing = false;
		arg_data->global_state = NULL;
		arg_data->in_dquotes = false;
		arg_data->option = false;
		arg_data->tmp = NULL;
		arg_data->raw = content;
	}
	return ((void *)arg_data);
}

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
		debug_print("\t_parse_args adding arg:%s\n", arg->raw);
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
	{
		cmd_node->data.cmd.name = ft_strdup(tok_get_raw(advance(p)));
		if (NULL == cmd_node->data.cmd.name)
			return (destroy_cmd_node(cmd_node), err("Malloc\n"), NULL);
	}
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
	cmd_node->data.cmd.do_expansion = false;
	cmd_node->data.cmd.do_globbing = false;
	cmd_node->data.cmd.do_redir_globbing = false;

}

/* Parsing of atomic command. */
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
