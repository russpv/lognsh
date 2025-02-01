#include "parse_int.h"

t_ast_node	*parse_logical(t_parser *p)
{
	t_ast_node	*log_node;
	t_list		*first;
	t_ast_node	*next_cmd;
	t_list		*next;
	t_list		*op;

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
			op = ft_lstnew(tok_get_raw(peek(p))); // store the op token
			if (op)
			{
				ft_lstadd_back(&log_node->data.log.operators, op);
			}
			else
			{
				err("Memory allocation error while creating operator node\n");
				free(log_node);
				return (NULL);
			}			
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
				return (NULL);
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
