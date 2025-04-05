#include "parse_int.h"

/* Returns new AST cmd_node */
static t_ast_node	*_init_cmd_node(t_mem_mgr *m)
{
	t_ast_node	*node;

	node = m->f(&m->list, sizeof(struct s_node));
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
		node->data.cmd.do_wordsplit = false;
		node->data.cmd.do_redir_globbing = false;
		node->data.cmd.do_redir_expansion = false;
		node->data.cmd.has_arggrouptoks = false;
		node->data.cmd.has_redgrouptoks = false;
	}
	return (node);
}

/* This helper consumes argument tokens and adds them to ast node's
 * linked list.
 * Returns -1 if nothing processed
 */
static int	_parse_args(t_parser *p, t_ast_node *cmd_node)
{
	t_arg_data	*arg;
	t_list		*new;
	int			initval;

	if (!p || !cmd_node)
		return (ERR_ARGS);
	initval = cmd_node->data.cmd.argc;
	while (!is_at_end(p) && is_arg_token(peek(p)))
	{
		arg = init_arg(p->mmgr, p, cmd_node, advance(p));
		if (NULL == arg)
			exit_clean(&p->mmgr->list, ENOMEM, __FUNCTION__, EMSG_MALLOC);
		new = ft_lstnew_tmp(p->mmgr, arg);
		if (NULL == new)
			exit_clean(&p->mmgr->list, ENOMEM, __FUNCTION__, EMSG_MALLOC);
		debug_print(_MOD_ ": %s: adding arg:%s\n", __FUNCTION__, arg->raw);
		ft_lstadd_back(&cmd_node->data.cmd.args, new);
		test_prev_integrity(cmd_node->data.cmd.args);
		cmd_node->data.cmd.argc++;
	}
	debug_print(_MOD_ ": %s: done on %d arg(s)\n", __FUNCTION__,
		cmd_node->data.cmd.argc);
	if (cmd_node->data.cmd.argc > initval)
		return (0);
	return (-1);
}

/* This helper consumes any redirection tokens,
	before or after the command name,
 * and its following arguments, if any.
 * If the command name is an expansion, handled later in Command module.
 * In case of parsing failure, frees any downstream memory and returns.
 * Does nothing to args.
 */
static int	_process_cmd(t_state *s, t_parser *p, t_ast_node *cmd_node)
{
	int res2;
	int res;

	if (!p || !cmd_node)
		return (ERR_ARGS);
	res = 0;
	while (res == 0)
	{
		res = process_redir(p, cmd_node);
		if (is_error(res))
			return (res);
	}
	if (false == is_cmd_token(peek(p)))
	{
		set_exit_status(s, EX_ERNDM);
		print_nocmd_error(s, tok_get_raw(previous(p)), tok_get_pos(peek(p)));
		return (err("Expected a command token, but none found\n"), ERR_SYNTAX);
	}
	if (false == is_expansion(peek(p)) && false == is_group_token(peek(p)))
	{
		cmd_node->data.cmd.name = ft_strdup_tmp(p->mmgr,
				tok_get_raw(advance(p)));
		if (NULL == cmd_node->data.cmd.name)
			exit_clean(&get_mem(s)->list, ENOMEM, __FUNCTION__, EMSG_MALLOC);
	}
	res = 0;
	while (res > -2)
	{
		res = 0;
		res = _parse_args(p, cmd_node);
		if (res > 0)
			return (ERR_GENERAL);
		res2 = process_redir(p, cmd_node);
		if (is_error(res2))
			return (res2);
		if (-1 == res2)
			res--;
	}
	return (0);
}

/* PARSE_CMD
 * Creates node of type AST_NODE_CMD
 * and adds it to AST
 */
t_ast_node	*parse_cmd(t_state *s, t_parser *p)
{
	t_ast_node	*ast_node;
	int			res;

	st_int_push(p->st, AST_NODE_CMD);
	debug_print(_MOD_ ": %s: tok: %s\n", __FUNCTION__, tok_get_raw(peek(p)));
	ast_node = _init_cmd_node(p->mmgr);
	if (NULL == ast_node)
		return (err(EMSG_MALLOC), NULL);
	res = _process_cmd(s, p, ast_node);
	if (0 != res)
	{
		set_error(s, res);
		if (get_status(s) == EX_HAPPY)
			set_exit_status(s, EX_ERNDM);
		//else
	//		set_exit_status(s, EX_EINVAL);
		destroy_ast_node(get_mem(s), (void **)&ast_node);
		return (NULL);
	}
	p->last_node = ast_node;
	st_int_pop(p->st);
	debug_print(_MOD_ ": %s: node args @ %p\n", __FUNCTION__,
		p_get_args(ast_node));
	debug_print(_MOD_ ": %s: %s done\n", __FUNCTION__, ast_node->data.cmd.name);
	return (ast_node);
}
