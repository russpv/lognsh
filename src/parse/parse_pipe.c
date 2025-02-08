#include "parse_int.h"

static t_ast_node	*_init_pipe(void)
{
	t_ast_node	*pipe_node;

	pipe_node = malloc(sizeof(t_ast_node));
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

	cmd_node = ft_lstnew(p->last_node);
	if (cmd_node)
	{
		ft_lstadd_back(&pipe_node->data.pipe.cmds, cmd_node);
		pipe_node->data.pipe.cmdc++;
	}
	else
	{
		err("Memory allocation error while creating pipe's command node\n");
		return (ERR_MEM);
	}
	return (0);
}

static int	_process_pipe(t_parser *p, t_ast_node *pipe_node)
{
	if (!p || !pipe_node)
		return (ERR_ARGS);
	if (NULL == p->last_node)
		return (ERR_SYNTAX);
	if (0 != _process_cmd(p, pipe_node))
		return (ERR_GENERAL);
	while (!is_at_end(p) && is_pipe_token(peek(p)))
	{
		advance(p);
		debug_print("Parser: parsing pipeline: getting next cmd...\n");
		if (NULL == parse_full_cmd(p))
			return (err("Failed to parse command after logical op\n"), ERR_GENERAL);
		if (0 != _process_cmd(p, pipe_node))
			return (ERR_GENERAL);
	}
	return (0);
}

/* Adds parsed cmds to a pipeline. First cmd must be parsed already. */
t_ast_node	*parse_pipeline(t_parser *p)
{
	t_ast_node	*ast_node;

	if (!p || !p->last_node)
		return (NULL);
	st_push(p->st, AST_NODE_PIPELINE);
	debug_print("Parser: parse_pipeline tok: %s\n", tok_get_raw(peek(p)));
	ast_node = _init_pipe();
	if (NULL == ast_node)
		return(err("Allocation failed for cmd node\n"), NULL);
	if (0 != _process_pipe(p, ast_node))
	{
		destroy_ast_node(ast_node);
		return (NULL);
	}
	debug_print("Parser: parsed pipeline of %d cmds\n", ast_node->data.pipe.cmdc);
	p->last_node = ast_node;
	st_pop(p->st);
	return (ast_node);
}
