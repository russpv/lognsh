#include "parse_int.h"

#define EM_PP_M "Memory allocation error while creating pipe's command node\n"
#define EMSG_PPIPE_SYNTAX "Failed to parse command after pipe operator\n"
#define EMSG_PPIPE_GEN "Pipeline parsing error\n"

static t_ast_node	*_init_pipe(t_mem_mgr *m)
{
	t_ast_node	*pipe_node;

	pipe_node = m->f(&m->list, sizeof(struct s_node));
	if (pipe_node)
	{
		pipe_node->type = AST_NODE_PIPELINE;
		pipe_node->data.pipe.cmds = NULL;
		pipe_node->data.pipe.cmdc = 0;
	}
	return (pipe_node);
}

static int	_process_cmd(t_parser *p, t_ast_node *pipe_node)
{
	t_list	*cmd_node;

	cmd_node = ft_lstnew_tmp(p->mmgr, p->last_node);
	if (cmd_node)
	{
		ft_lstadd_back(&pipe_node->data.pipe.cmds, cmd_node);
		pipe_node->data.pipe.cmdc++;
	}
	else
		exit_clean(&p->mmgr->list, ENOMEM, __FUNCTION__, EM_PP_M);
	return (0);
}

/* This adds the previously parsed node as the first command,
 * continues adding commands for each pipe token found.
 */
static int	_process_pipe(t_state *s, t_parser *p, t_ast_node *pipe_node)
{
	if (!p || !pipe_node)
		return (ERR_ARGS);
	if (NULL == p->last_node || TOK_PIPE == tok_get_type(previous(p)))
	{
		print_parse_error(s, tok_get_raw((p->curr_tok)->content),
			tok_get_pos((p->curr_tok)->content));
		return (p->parse_error = true, ERR_SYNTAX);
	}
	if (0 != _process_cmd(p, pipe_node))
		return (ERR_GENERAL);
	while (!is_at_end(p) && is_pipe_token(peek(p)))
	{
		advance(p);
		dprint("%s: %s: getting next cmd...\n", _MOD_, __FUNCTION__);
		if (NULL == parse_full_cmd(s, p))
		{
			print_parse_error(s, tok_get_raw((p->curr_tok)->prev->content),
				tok_get_pos((p->curr_tok)->content));
			return (err(EMSG_PPIPE_SYNTAX), p->parse_error = true, ERR_SYNTAX);
		}
		if (0 != _process_cmd(p, pipe_node))
			return (ERR_GENERAL);
	}
	return (0);
}

/* Adds parsed cmds to a pipeline. First cmd must be parsed already. */
t_ast_node	*parse_pipeline(t_state *s, t_parser *p)
{
	t_ast_node	*ast_node;
	int			res;

	if (!p || !p->last_node)
		return (NULL);
	st_int_push(p->st, AST_NODE_PIPELINE);
	dprint("%s: %s: tok: %s\n", _MOD_, __FUNCTION__, tok_get_raw(peek(p)));
	ast_node = _init_pipe(p->mmgr);
	if (NULL == ast_node)
		exit_clean(&p->mmgr->list, ENOMEM, __FUNCTION__, EM_PP_M);
	res = _process_pipe(s, p, ast_node);
	if (0 != res)
	{
		set_error(s, res);
		destroy_ast_node(get_mem(s), (void **)&ast_node);
		p->parse_error = true;
		return (err(EMSG_PPIPE_GEN), NULL);
	}
	dprint("%s: %s: parsed %d cmds\n", _MOD_, __FUNCTION__,
		ast_node->data.pipe.cmdc);
	p->last_node = ast_node;
	st_int_pop(p->st);
	return (ast_node);
}
