#include "parse_int.h"

// Returns new AST cmd_node
static t_ast_node	*_init_cmd_node(void)
{
	t_ast_node *node = malloc(sizeof(struct s_node));
	if (node)
	{
		node->type = AST_NODE_CMD;
		node->data.cmd.args = NULL;
		node->data.cmd.redirs = NULL;
		node->data.cmd.redc = 0;
		node->data.cmd.argc = 0;
		node->data.cmd.name = NULL;
		node->data.cmd.do_expansion = false;
		node->data.cmd.do_globbing = false;
		node->data.cmd.do_redir_globbing = false;
	}
	return (node);
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
static t_arg_data	*_init_arg(t_parser *p, t_ast_node *cmd_node, t_tok *tok)
{
	t_arg_data *arg;

	arg = malloc(sizeof(struct s_arg));
	if (arg)
	{
		arg->raw = ft_strdup(tok_get_raw(tok));
		if (!arg->raw)
		{
			err("Allocation failed for arg\n");
			free(arg);
			return (NULL);
		}	
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
	return (arg);
}

/* This helper consumes argument tokens and adds them to ast node's 
 * linked list.
 * Returns NULL if syntax error.
 */
static int	_parse_args(t_parser *p, t_ast_node *cmd_node)
{
	t_arg_data	*arg;
	t_list		*new;

	while (!is_at_end(p) && is_arg_token(peek(p)))
	{
		arg = _init_arg(p, cmd_node, advance(p));
		if (NULL == arg)
			return (ERR_MEM);
		new = ft_lstnew(arg);
		if (!new)
		{
			err("Memory allocation error adding arg node\n");
			free(arg);
			return (ERR_MEM);
		}
		debug_print("Parser: \t_parse_args adding arg:%s\n", arg->raw);
		ft_lstadd_back(&cmd_node->data.cmd.args, new);
		cmd_node->data.cmd.argc++;
	}
	return (0);
}

/* This helper consumes any redirection tokens, before or after the command name,
 * and its following arguments, if any.
 * If the command name is an expansion, handled later in Command module.
 * In case of parsing failure, frees any downstream memory and returns.
 * Does nothing to args.
 */
static int	_process_cmd(t_parser *p, t_ast_node *cmd_node)
{	
	if (0 != process_redir(p, cmd_node))
		return (ERR_GENERAL);
	if (false == is_cmd_token(peek(p)))
		return (err("Expected a command token, but none found\n"), ERR_SYNTAX);
	if (false == is_expansion(peek(p)))
	{
		cmd_node->data.cmd.name = ft_strdup(tok_get_raw(advance(p)));
		if (NULL == cmd_node->data.cmd.name)
			return (err("Malloc\n"), ERR_MEM);
	}
	if (0 != _parse_args(p, cmd_node))
		return (ERR_GENERAL);
	if (0 != process_redir(p, cmd_node))
		return (ERR_GENERAL);
	return (0);
}

/* PARSE_CMD
 * Creates node of type AST_NODE_CMD 
 * and adds it to AST
 */
t_ast_node	*parse_cmd(t_parser *p)
{
	t_ast_node	*ast_node;

	st_push(p->st, AST_NODE_CMD);
	debug_print("Parser: parse_cmd tok: %s\n", tok_get_raw(peek(p)));
	ast_node = _init_cmd_node();
	if (0 != _process_cmd(p, ast_node))
	{
		destroy_ast_node(ast_node);
		return (NULL);
	}
	p->last_node = ast_node;
	st_pop(p->st);
	return (ast_node);
}
