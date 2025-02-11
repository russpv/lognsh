#include "parse_int.h"

// Allocates new ast node on the heap
static t_ast_node	*_init_proc(void)
{
	t_ast_node	*proc_node;

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

/* Stores command. Assumes parser is on
 * correct token.
 */
static int	_process_cmd(t_parser *p, t_ast_node *proc_node)
{
	t_list	*cmd_node;

	if (!p || !proc_node)
		return (ERR_ARGS);
	debug_print("Parser: Proc got a node of type %d\n", p->last_node->type);
	cmd_node = ft_lstnew(p->last_node);
	if (cmd_node)
	{
		ft_lstadd_back(&proc_node->data.proc.cmds, cmd_node);
		proc_node->data.proc.cmdc++;
	}
	else
	{
		err("Memory allocation error while creating proc's command node\n");
		return (ERR_MEM);
	}
	return (0);
}
/* Consumes open ( and adds command(s) */
static int	_process_proc(t_parser *p, t_ast_node *proc_node)
{
	if (!p || !proc_node)
		return (ERR_ARGS);
	if (!is_open_paren(peek(p)))
		return (err("Expected '(' to start a process\n"), ERR_SYNTAX);
	advance(p);
	while (!is_at_end(p) && !is_close_paren(peek(p)))
	{
		debug_print("Parser: parsing proc: getting next cmd...\n");
		if (NULL == parse_full_cmd(p))
			return (err("Failed to parse proc command\n"), ERR_GENERAL);
		if (0 != _process_cmd(p, proc_node))
			return (ERR_GENERAL);
	}
	if (!is_close_paren(peek(p)))
		return (err("Expected ')' to close process\n"), ERR_SYNTAX);
	advance(p);
	return (0);
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
	t_ast_node	*ast_node;

	st_push(p->st, AST_NODE_PROC);
	debug_print("Parser: parse_proc tok: %s\n", tok_get_raw(peek(p)));
	ast_node = _init_proc();
	if (NULL == ast_node)
		return (err("Allocation failed for proc node\n"), NULL);
	if (0 != _process_proc(p, ast_node))
	{
		destroy_ast_node(ast_node);
		return (NULL);
	}
	debug_print("Parser: curr peek tok: %s\n", tok_get_raw(peek(p)));
	debug_print("Parser: parse_proc doing redirs\n");
	process_redir(p, ast_node); // TODO test this
	p->last_node = ast_node;
	st_pop(p->st);
	debug_print("Parser: parsed proc of %d cmds\n", ast_node->data.proc.cmdc);
	debug_print("Parser: curr peek tok: %s\n", tok_get_raw(peek(p)));
	return (ast_node);
}
