#include "parse_int.h"

#define ERRMSG_LOGI_PARSE "Failed to parse command after logical op\n"
#define ERRMSG_LOGI_NODE_MALLOC "Allocation failed for logical node\n"
#define ERRMSG_LOGI_CMD_MALLOC "Memory allocation error while creating logical's command node\n"
#define ERRMSG_LOGI_OP_MALLOC "Memory allocation error while creating operator node\n"

/* Stores command. Assumes parser is on
 * correct token.
 */
static int	_process_cmd(t_parser *p, t_ast_node *log_node)
{
	t_list	*cmd_node;

	if (!p || !log_node)
		return (ERR_ARGS);
	cmd_node = ft_lstnew(p->last_node);
	if (cmd_node)
	{
		ft_lstadd_back(&log_node->data.log.cmds, cmd_node);
		log_node->data.log.cmdc++;
	}
	else
	{
		err(ERRMSG_LOGI_CMD_MALLOC);
		return (ERR_MEM);
	}
	return (0);
}

/* Stores the operator token. Assumes parser is on
 * correct token.
 */
static int	_process_op(t_parser *p, t_ast_node *log_node)
{
	t_list	*op;

	if (!p || !log_node)
		return (ERR_ARGS);
	op = ft_lstnew_copystr(tok_get_raw(peek(p)), ft_strdup);
	if (op)
		ft_lstadd_back(&log_node->data.log.operators, op);
	else
	{
		err(ERRMSG_LOGI_OP_MALLOC);
		return (ERR_MEM);
	}
	return (0);
}

static int	_do_ops(t_state *s, t_parser *p, t_ast_node *log_node)
{
	if (NULL == parse_full_cmd(s, p))
		return (err(ERRMSG_LOGI_PARSE), ERR_GENERAL);
	if (peek(p) == which_lower_priority(log_node, peek(p)))
	{
		if (0 != _process_cmd(p, log_node))
			return (ERR_GENERAL);
	}
	else
	{
		if (NULL == parse_full_cmd(s, p))
			return (err(ERRMSG_LOGI_PARSE), ERR_GENERAL);
		if (0 != _process_cmd(p, log_node))
			return (ERR_GENERAL);
	}
	return (0);
}

/* Adds commands and logical operator to logical ast_node
 * Must have parsed an ast_node prior.
 * Determines whether to add the following command or to
 * let it associate with higher priority next operator.
 */
static int	_process_log(t_state *s, t_parser *p, t_ast_node *log_node)
{
	int	res;

	if (!p || !log_node)
		return (ERR_ARGS);
	if (NULL == p->last_node)
		return (ERR_SYNTAX);
	if (0 != _process_cmd(p, log_node))
		return (ERR_GENERAL);
	while (!is_at_end(p) && is_log_token(peek(p)))
	{
		if (0 != _process_op(p, log_node))
			return (ERR_GENERAL);
		advance(p);
		debug_print("Parser: parsing logical: getting next cmd...\n");
		res = _do_ops(s, p, log_node);
		if (0 != res)
			return (res);
	}
	return (0);
}

/* PARSE LOGICAL
 *
 */
t_ast_node	*parse_logical(t_state *s, t_parser *p)
{
	t_ast_node	*ast_node;
	int			res;

	st_int_push(p->st, AST_NODE_LOG);
	debug_print("Parser: parse_logical tok: %s\n", tok_get_raw(peek(p)));
	ast_node = init_log();
	if (NULL == ast_node)
		return (err(ERRMSG_LOGI_NODE_MALLOC), NULL);
	res = _process_log(s, p, ast_node);
	if (0 != res)
	{
		destroy_ast_node(ast_node);
		set_error(s, res);
		return (NULL);
	}
	p->last_node = ast_node;
	st_int_pop(p->st);
	debug_print("Parser: parsed logical of %d cmds\n", ast_node->data.log.cmdc);
	return (ast_node);
}
