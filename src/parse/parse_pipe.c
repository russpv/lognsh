#include "parse_int.h"

/* Adds parsed cmds to a pipeline. First cmd must be parsed already. */
t_ast_node	*parse_pipeline(t_parser *p)
{
	t_ast_node	*pipe_node;
	t_list		*first;
	t_ast_node	*next_cmd;
	t_list		*next;

	if (!p->last_node)
		return (NULL);
	debug_print("parsing pipeline\n");
	st_push(p->st, AST_NODE_PIPELINE);
	pipe_node = malloc(sizeof(t_ast_node));
	if (pipe_node)
	{
		pipe_node->type = AST_NODE_PIPELINE;
		pipe_node->data.pipe.cmds = NULL;
		pipe_node->data.pipe.cmdc = 0;
		while (!is_at_end(p) && tok_get_type(peek(p)) == TOK_PIPE)
		{
			advance(p);
			first = ft_lstnew(p->last_node);
			if (first)
			{
				ft_lstadd_back(&pipe_node->data.pipe.cmds, first);
				pipe_node->data.pipe.cmdc++;
			}
			else
			{
				err("Memory allocation error while creating the first command node\n");
				free(pipe_node);
				return (NULL); // Return NULL on error
			}
			debug_print("parsing pipeline: getting next cmd...\n");
			next_cmd = parse_full_cmd(p);
			if (!next_cmd)
			{
				free(pipe_node);
				err("Failed to parse command after pipe\n");
				return (NULL);
			}
		}
		next = ft_lstnew(p->last_node);
		if (next)
		{
			ft_lstadd_back(&pipe_node->data.pipe.cmds, next);
			pipe_node->data.pipe.cmdc++;
		}
		else
		{
			err("Memory allocation error while creating subsequent pipe node\n");
			free(pipe_node);
			return (NULL); // Return NULL on error
		}
	}
	else
		err("Memory allocation error on pipe node\n");
	debug_print("parsed pipeline of %d cmds\n", pipe_node->data.pipe.cmdc);
	p->last_node = pipe_node;
	st_pop(p->st);
	return (pipe_node);
}
