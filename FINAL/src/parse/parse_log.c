#include "parse_int.h"

#define EMSG_LOGI_PARSE "Failed to parse command after logical op\n"
#define EMSG_LOGI_NODE_MALLOC "Allocation failed for logical node\n"
#define EMSG_LMEM "Malloc error while creating logical's command node\n"
#define EMSG_LMEMO "Malloc error while creating operator node\n"

t_ast_node	*init_log(t_mem_mgr *m)
{
	t_ast_node	*log_node;

	log_node = m->f(&m->list, sizeof(struct s_node));
	if (log_node)
	{
		log_node->type = AST_NODE_LOG;
		log_node->data.log.cmds = NULL;
		log_node->data.log.cmdc = 0;
		log_node->data.log.operators = NULL;
	}
	return (log_node);
}

/* Returns cmp if it is a lower priority operator than
 * that represented by ref. In case of tie, 
 */
t_tok	*which_lower_priority(t_ast_node *ref, t_tok *cmp)
{
	if (!ref | !cmp)
		return (NULL);
	if (is_close_paren(cmp) || tok_get_type(cmp) == TOK_EOF)
		return (cmp);
	if (is_group_op_token(cmp))
	{
		if (ref->type == AST_NODE_PIPELINE && is_log_token(cmp))
			return (cmp);
	}
	return (NULL);
}

/* Stores command. Assumes parser is on
 * correct token.
 */
static int	_commit_cmd(t_parser *p, t_ast_node *log_node)
{
	t_list	*cmd_node;

	if (!p || !log_node)
		return (ERR_ARGS);
	cmd_node = ft_lstnew_tmp(p->mmgr, p->last_node);
	if (cmd_node)
	{
		ft_lstadd_back(&log_node->data.log.cmds, cmd_node);
		log_node->data.log.cmdc++;
	}
	else
		exit_clean(&p->mmgr->list, ENOMEM, __FUNCTION__, EMSG_LMEM);
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
	op = ft_lstnew_copystr_mem(p->mmgr, tok_get_raw(peek(p)), ft_strdup_mem);
	if (op)
		ft_lstadd_back(&log_node->data.log.operators, op);
	else
		exit_clean(&p->mmgr->list, ENOMEM, __FUNCTION__, EMSG_LMEMO);
	return (0);
}

// Note: the next node cannot be a logical operator
static int	_do_ops(t_state *s, t_parser *p, t_ast_node *log_node)
{
	t_ast_node *next;

	if (is_log_token(peek(p)))
		return (err(EMSG_LOGI_PARSE), ERR_GENERAL);
	next = parse_full_cmd(s, p);
	if (NULL == next || AST_NODE_LOG == p_get_type(next))
		return (err(EMSG_LOGI_PARSE), ERR_GENERAL);
	if (peek(p) == which_lower_priority(log_node, peek(p)))
	{
		if (0 != _commit_cmd(p, log_node))
			return (ERR_GENERAL);
	}
	else
	{
		if (NULL == parse_full_cmd(s, p))
			return (err(EMSG_LOGI_PARSE), ERR_GENERAL);
		if (0 != _commit_cmd(p, log_node))
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
	if (NULL == p->last_node || get_err(s))
		return (ERR_SYNTAX);
	if (0 != _commit_cmd(p, log_node))
		return (ERR_GENERAL);
	while (!is_at_end(p) && is_log_token(peek(p)))
	{
		if (0 != _process_op(p, log_node))
			return (ERR_GENERAL);
		advance(p);
		dprint(_MOD_ ": parsing logical: getting next cmd...\n");
		res = _do_ops(s, p, log_node);
		if (0 != res)
			return (res);
	}
	return (0);
}

/* PARSE LOGICAL
 */
t_ast_node	*parse_logical(t_state *s, t_parser *p)
{
	t_ast_node	*ast_node;
	int			res;

	st_int_push(p->st, AST_NODE_LOG);
	dprint(_MOD_ ": parse_logical tok: %s\n", tok_get_raw(peek(p)));
	ast_node = init_log(p->mmgr);
	if (NULL == ast_node)
		exit_clean(&p->mmgr->list, ENOMEM, __FUNCTION__, EMSG_LOGI_NODE_MALLOC);
	res = _process_log(s, p, ast_node);
	if (0 != res)
	{
		if (!get_err(s))
			print_gen_err(EMSG_LOGI_PARSE);
		destroy_ast_node(get_mem(s), (void **)&ast_node);
		set_error(s, res);
		p->parse_error = true;
		return (NULL);
	}
	p->last_node = ast_node;
	st_int_pop(p->st);
	dprint(_MOD_ ": parsed logical of %d cmds\n", ast_node->data.log.cmdc);
	return (ast_node);
}
