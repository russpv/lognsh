
#include "parse_int.h"

#define EM_HDC_M "Memory allocation failed for redirection heredoc\n"
#define EM_RDR_M "Failed to create execution node for redirection\n"
#define EMSG_REDIR_FAIL "Failed to parse redirection\n"

/* Creates new redir node and adds to t_list.
 * Freeing handled by caller.
 */
static int	_add_redir(t_mem_mgr *m, t_ast_node *node, t_redir_data *red)
{
	t_list	*new;

	if (!red || !node)
		return (ERR_ARGS);
	new = ft_lstnew_tmp(m, red);
	if (!new)
		exit_clean(&m->list, ENOMEM, __FUNCTION__, EM_RDR_M);
	dprint(_MOD_ ": Adding redirection: (%s %s | doc:%s glob:%d exp:%d)\n",
		red->symbol, red->filename, red->heredoc_body, red->do_globbing,
		red->do_expansion);
	ft_lstadd_back(p_get_redirs_ptr(node), new);
	p_update_redc(node, 1);
	return (0);
}

/* Copies TOK_HEREDOC_WORD content to redir data.
 * Returns ERR_MEM if malloc fails.
 * Freeing handled by caller.
 */
static int	_process_heredoc_redir(t_parser *p, t_tok *tok, t_redir_data *red,
		t_ast_node *n)
{
	if (!red || !tok || !n)
		return (ERR_ARGS);
	if (p_get_type(n) != AST_NODE_CMD)
		return (ERR_ARGS);
	dprint(_MOD_ ": Got here document\n");
	red->symbol = NULL;
	red->filename = NULL;
	red->do_expansion = tok_get_expansion(tok);
	p_set_do_redir_expansion(n, red->do_expansion);
	red->do_globbing = false;
	red->heredoc_body = ft_strdup_mem(p->mmgr, tok_get_raw((t_tok *)tok));
	if (!red->heredoc_body)
		exit_clean(&p->mmgr->list, ENOMEM, __FUNCTION__, EM_HDC_M);
	return (0);
}

/* Consumes redir tokens and adds them to node (arg 2) linked list
 * Builds group redirs if the next token is a group token
 * Returns NULL if syntax error
 * Heredocs already turned into TOK_HEREDOC_WORD
 */
static int	_parse_redir(t_parser *p, t_ast_node *node)
{
	t_redir_data	*red;
	t_tok			*tok;

	if (!p || !node)
		return (ERR_ARGS);
	while (!is_at_end(p) && is_redir_token(peek(p)))
	{
		tok = advance(p);
		red = init_redir(p->mmgr, p, node, tok_get_type(tok));
		if (false == is_heredoc_token((t_tok *)tok))
		{
			if (0 != process_normal_redir(p, tok, red, node))
				return (destroy_redir(p->mmgr, (void **)&red), ERR_GENERAL);
		}
		else
		{
			if (0 != _process_heredoc_redir(p, tok, red, node))
				return (destroy_redir(p->mmgr, (void **)&red), ERR_GENERAL);
		}
		if (0 != _add_redir(p->mmgr, node, red))
			return (destroy_redir(p->mmgr, (void **)&red), ERR_GENERAL);
	}
	dprint(_MOD_ ": _parse_redir: curr peek tok: %s\n",
		tok_get_raw(peek(p)));
	return (0);
}

/* PROCESS REDIR
 * Adds redirections to node's t_list
 * Upon failure, frees any downstream heap mem
 * and returns. Does nothing to args.
 * Returns -1 if nothing processed.
 */
int	process_redir(t_parser *p, t_ast_node *ast_node)
{
	if (is_redir_token(peek(p)))
	{
		if (0 != _parse_redir(p, ast_node))
		{
			err(EMSG_REDIR_FAIL);
			set_exit_status(p->global_state, EX_EREDIR);
			return (ERR_GENERAL);
		}
		return (0);
	}
	else
		dprint(_MOD_ ": Not a redir:%s\n", tok_get_raw(peek(p)));
	return (-1);
}
