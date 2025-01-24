#include "parse_int.h"

/* Current token is a '(' */
t_ast_node	*parse_proc(t_parser *p)
{
	t_ast_node	*proc_node;
	t_ast_node	*cmd_node;
	t_list		*new_cmd;

	debug_print("parsing PROC\n");
	st_push(p->st, AST_NODE_PROC);
	p->ref_node = p->last_node;
	if (tok_get_type(peek(p)) != TOK_OPEN_PAREN)
	{
		err("Expected '(' to start a process\n");
		return (NULL);
	}
	advance(p);
	proc_node = malloc(sizeof(t_ast_node));
	if (!proc_node)
	{
		err("Memory allocation error for process node\n");
		return (NULL);
	}
	proc_node->type = AST_NODE_PROC;
	proc_node->data.proc.cmds = NULL;
	proc_node->data.proc.cmdc = 0;
	while (!is_at_end(p) && tok_get_type(peek(p)) != TOK_CLOSE_PAREN)
	{
		cmd_node = parse_full_cmd(p);
		if (!cmd_node)
		{
			free(proc_node);
			err("Failed to parse command inside process\n");
			return (NULL);
		}
		debug_print("Proc got a node of type %d\n", cmd_node->type);
	}
	if (tok_get_type(peek(p)) != TOK_CLOSE_PAREN)
	{
		free(proc_node);
		err("Expected ')' to close process\n");
		return (NULL);
	}
	if (p->ref_node != p->last_node)
	{
		new_cmd = ft_lstnew(p->last_node);
		if (!new_cmd)
		{
			free(proc_node);
			err("Memory allocation error while adding command to process\n");
			return (NULL);
		}
		ft_lstadd_back(&proc_node->data.proc.cmds, new_cmd);
		proc_node->data.proc.cmdc++;
	}
	advance(p);
	p->last_node = proc_node;
	debug_print("parsed proc of %d cmds\n", proc_node->data.proc.cmdc);
	st_pop(p->st);
	p->last_node = proc_node;
	return (proc_node);
}

//
t_ast_node	*parse_logical(t_parser *p)
{
	t_ast_node	*log_node;
	t_list		*first;
	t_ast_node	*next_cmd;
	t_list		*next;

	if (!p->last_node) // need to have something parsed already
		return (NULL);
	debug_print("parsing logical\n");
	st_push(p->st, AST_NODE_LOG);
	log_node = malloc(sizeof(t_ast_node));
	if (log_node)
	{
		log_node->type = AST_NODE_LOG;
		log_node->data.log.cmds = NULL;
		log_node->data.log.cmdc = 0;
		log_node->data.log.operators = NULL;
		while (!is_at_end(p) && is_log_token(peek(p)))
		{
			advance(p);
			first = ft_lstnew(p->last_node);
			if (first)
			{
				ft_lstadd_back(&log_node->data.log.cmds, first);
				log_node->data.log.cmdc++;
			}
			else
			{
				err("Memory allocation error while creating the first command node\n");
				free(log_node);
				return (NULL); // Return NULL on error
			}
			debug_print("parsing logical: getting next cmd...\n");
			next_cmd = parse_full_cmd(p);
			if (!next_cmd)
			{
				free(log_node);
				err("Failed to parse command after logical\n");
				return (NULL);
			}
		}
		next = ft_lstnew(p->last_node);
		if (next)
		{
			ft_lstadd_back(&log_node->data.log.cmds, next);
			log_node->data.log.cmdc++;
		}
		else
		{
			err("Memory allocation error while creating subsequent pipe node\n");
			free(log_node);
			return (NULL); // Return NULL on error
		}
	}
	debug_print("parsed logical of %d cmds\n", log_node->data.log.cmdc);
	st_pop(p->st);
	p->last_node = log_node;
	return (log_node);
}

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

