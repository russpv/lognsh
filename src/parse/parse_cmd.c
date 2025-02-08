#include "parse_int.h"

// Takes generic ast_node, creates cmd_node in it
static void	_init_cmd_node(t_ast_node *ast_node)
{
	ast_node->type = AST_NODE_CMD;
	ast_node->data.cmd.args = NULL;
	ast_node->data.cmd.redirs = NULL;
	ast_node->data.cmd.redc = 0;
	ast_node->data.cmd.argc = 0;
	ast_node->data.cmd.do_expansion = false;
	ast_node->data.cmd.do_globbing = false;
	ast_node->data.cmd.do_redir_globbing = false;
}

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

/* Must deep copy token strings to decouple token-list/ast. */
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

/* This helper consumes argument tokens and adds them to ast node's 
 * linked list.
 * Returns NULL if syntax error.
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
		debug_print("Parser: \t_parse_args adding arg:%s\n", arg->raw);
		ft_lstadd_back(&cmd_node->data.cmd.args, new);
		cmd_node->data.cmd.argc++;
	}
	return (cmd_node->data.cmd.args);
}

/* This helper consumes any redirection tokens, before or after the command name,
 * and its following arguments, if any.
 * If the command name is an expansion, handled later in Commmand module.
 */
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

/* PARSE_CMD
 * Creates node of type AST_NODE_CMD 
 * and adds it to AST
 */
t_ast_node	*parse_cmd(t_parser *p)
{
	t_ast_node	*ast_node;

	st_push(p->st, AST_NODE_CMD);
	ast_node = malloc(sizeof(struct s_node));
	if (ast_node)
	{
		debug_print("Parser: cmd tok: %s\n", tok_get_raw(peek(p)));
		_init_cmd_node(ast_node);
		if (!_process_cmd(p, ast_node))
			return (NULL);
	}
	else
	{
		err("Memory allocation failed for command node\n");
		return (NULL);
	}
	p->last_node = ast_node;
	st_pop(p->st);
	return (ast_node);
}
