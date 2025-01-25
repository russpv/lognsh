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
