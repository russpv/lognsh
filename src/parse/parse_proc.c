#include "parse_int.h"

// Allocates new ast node on the heap
static t_ast_node	*init_proc(void)
{
	t_ast_node *proc_node;
	
	proc_node = malloc(sizeof(struct s_node));
	if (!proc_node)
	{
		err("Memory allocation error for process node\n");
		return (NULL);
	}
	proc_node->type = AST_NODE_PROC;
	proc_node->data.proc.cmds = NULL;
	proc_node->data.proc.cmdc = 0;
	proc_node->data.proc.redirs = NULL;
	proc_node->data.proc.redc = 0;
	return (proc_node);
}

/* PARSE PROC
 * Consumes tokens to create a subshell node on the AST.
 * Adds any redirections AFTER the parentheses.
 * Adds all sub-nodes as shallow copies to cmds t_list.
 * Current token must be a '('.
 * Must end on a ')' to add ONE subnode.
 */
t_ast_node	*parse_proc(t_parser *p)
{
	t_ast_node	*proc_node;
	t_ast_node	*cmd_node;
	t_list		*new_cmd;

	debug_print("Parser: parsing PROC\n");
	st_push(p->st, AST_NODE_PROC);
	p->ref_node = p->last_node;
	if (tok_get_type(peek(p)) != TOK_OPEN_PAREN)
	{
		err("Expected '(' to start a process\n");
		return (NULL);
	}
	advance(p);
	proc_node = init_proc();
	if (!proc_node)
		return (NULL);
	while (!is_at_end(p) && tok_get_type(peek(p)) != TOK_CLOSE_PAREN)
	{
		cmd_node = parse_full_cmd(p); //sets p->last_node
		if (!cmd_node)
		{
			free(proc_node);
			err("Failed to parse command inside process\n");
			return (NULL);
		}
		debug_print("Parser: Proc got a node of type %d\n", cmd_node->type);
		// TODO error if multiple commands or nodes found
	}
	if (tok_get_type(peek(p)) != TOK_CLOSE_PAREN)
	{
		free(proc_node); //TODO free this properly
		err("Expected ')' to close process\n");
		return (NULL);
	}
	if (p->ref_node != p->last_node)
	{
		new_cmd = ft_lstnew(p->last_node);
		if (!new_cmd)
		{
			free(proc_node); //TODO free this properly
			err("Memory allocation error while adding command to process\n");
			return (NULL);
		}
		ft_lstadd_back(&proc_node->data.proc.cmds, new_cmd);
		proc_node->data.proc.cmdc++;
	}
	advance(p);
	process_redir(p, proc_node); //TODO test this
	p->last_node = proc_node;
	debug_print("Parser: parsed proc of %d cmds\n", proc_node->data.proc.cmdc);
	st_pop(p->st);
	p->last_node = proc_node;
	return (proc_node);
}
