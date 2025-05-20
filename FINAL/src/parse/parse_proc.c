#include "parse_int.h"

/*
** Adjusts the child node of p->last_node when a lower-priority grouping node
** is parsed after a higher-priority operator, which can occur due to infix
** operator grammar.
**
** The function processes operators based on their precedence:
** - If an operator with higher precedence is encountered, a swap is made.
** - If an operator with lower precedence is encountered, the function stops.
**
** Example behavior:
** - In expressions like `cmd && cmd2 | cmd3`, the swap happens because
**   `|` has higher precedence than `&&`.
** - In `cmd | cmd3 && cmd4`, parsing stops when `&&` is encountered
**   after the pipeline.
** - Complex expressions such as `(cmd && cmd2 | cmd3 && cmd4)` will result
**   in a series of swaps for logical and subordinate pipe node.
** - For `(cmd | cmd2 && cmd3 | cmd4)`, the parsing involves two swaps to
**   correctly identify the logical operator beneath the proc.
**
** Notes:
** - In `_add_cmd`, the next operator is checked after the parsed command.
**   	If it does not terminate the expression, (and it is a lower
** 		priority node) `last_node` is not added as the proc node child.
** - `_process_proc` will then parse the next node (as a grouping node) that
** 			associates with the higher-priority node just parsed. This node
** 		will not return until `)` is encountered, as it has the lowest priority.
** - For `process_log`, the same logic applies. `process_pipe` does
** 		not swap nodes since it works only on `proc` or `atom`, and `proc`
** 		is always prefixed (not infixed).
** - p->last_node is how information is obtained from sub-nodes.
*/

// Allocates new ast node on the heap
static t_ast_node	*_init_proc(t_mem_mgr *m)
{
	t_ast_node	*proc_node;

	proc_node = m->f(&m->list, sizeof(struct s_node));
	if (!proc_node)
		exit_clean(&m->list, ENOMEM, __FUNCTION__,
			"Memory allocation error for process node\n");
	proc_node->type = AST_NODE_PROC;
	proc_node->data.proc.cmds = NULL;
	proc_node->data.proc.cmdc = 0;
	proc_node->data.proc.redirs = NULL;
	proc_node->data.proc.redc = 0;
	proc_node->data.proc.has_redgrouptoks = false;
	proc_node->data.proc.do_redir_globbing = false;
	proc_node->data.proc.do_redir_expansion = false;
	return (proc_node);
}

// Stores command. Assumes parser is on correct token.
static int	_add_cmd(t_parser *p, t_ast_node *proc_node)
{
	t_list	*cmd_node;

	if (!p || !proc_node || !p->last_node)
		return (ERR_ARGS);
	dprint(_MOD_ ": Proc got a node of type %d\n", p->last_node->type);
	cmd_node = ft_lstnew_tmp(p->mmgr, p->last_node);
	if (cmd_node)
	{
		ft_lstadd_back(&proc_node->data.proc.cmds, cmd_node);
		proc_node->data.proc.cmdc++;
	}
	else
		exit_clean(&p->mmgr->list, ENOMEM, __FUNCTION__,
			"Memory allocation error while creating \
			proc's command node\n");
	return (0);
}

/* Consumes open (, adds the sub-command, consumes close )
 * Note: we don't need the loop with current grammar.
 * Remains for extension with ';'.
 */
static int	_process_proc(t_state *s, t_parser *p, t_ast_node *proc_node)
{
	if (!p || !proc_node)
		return (ERR_ARGS);
	if (!is_open_paren(peek(p)))
		return (err("Expected '(' to start a process\n"), ERR_SYNTAX);
	advance(p);
	while (!is_at_end(p) && !is_close_paren(peek(p)))
	{
		dprint(_MOD_ ": parsing proc: getting next cmd...\n");
		if (NULL == parse_full_cmd(s, p))
			return (err("Failed to parse proc command\n"), ERR_GENERAL);
	}
	if (!is_close_paren(peek(p)))
	{
		print_parse_error(s, tok_get_raw((p->curr_tok)->prev->content),
			tok_get_pos((p->curr_tok)->content));
		return (err("Expected ')' to close process\n"), ERR_SYNTAX);
	}
	if (0 != _add_cmd(p, proc_node))
		return (ERR_GENERAL);
	advance(p);
	return (0);
}

/* PARSE PROC
** Consumes tokens to create a subshell node on the AST.
** Adds any redirections AFTER the parentheses.
** Adds all sub-nodes as shallow copies to cmds t_list.
** Current token must be a '('.
** Must end on a ')' to add ONE subnode.
*/
t_ast_node	*parse_proc(t_state *s, t_parser *p)
{
	t_ast_node	*ast_node;
	int			res;

	st_int_push(p->st, AST_NODE_PROC);
	dprint(_MOD_ ": parse_proc tok: %s\n", tok_get_raw(peek(p)));
	ast_node = _init_proc(p->mmgr);
	if (NULL == ast_node)
		exit_clean(&p->mmgr->list, ENOMEM, __FUNCTION__,
			"Allocation failed for proc node\n");
	res = _process_proc(s, p, ast_node);
	if (0 != res)
	{
		set_error(s, res);
		destroy_ast_node(get_mem(s), (void **)&ast_node);
		return (NULL);
	}
	dprint(_MOD_ ": curr peek tok: %s\n", tok_get_raw(peek(p)));
	dprint(_MOD_ ": parse_proc doing redirs\n");
	process_redir(p, ast_node);
	p->last_node = ast_node;
	dprint(_MOD_ ": parsed proc of %d cmds\n", ast_node->data.proc.cmdc);
	dprint(_MOD_ ": curr peek tok: %s\n", tok_get_raw(peek(p)));
	st_int_pop(p->st);
	return (ast_node);
}
