#include "parse_int.h"

#define EMSG_REDIR_FN "Allocation for redirection filename failed\n"
#define EMSG_REDIR_FN_INVALID "Expected a valid filename after redirection operator\n"
#define EMSG_REDIR_FN_EOF "Expected a filename after redirection operator, found EOF\n"
#define EMSG_REDIR_SYM_MALLOC "Allocation for redirection symbol failed\n"
#define EMSG_HEREDOC_MALLOC "Memory allocation failed for redirection heredoc\n"
#define EMSG_REDIR_NODE_MALLOC "Failed to create execution node for redirection\n"
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
		exit_clean(&m->list, ENOMEM, __FUNCTION__, EMSG_REDIR_NODE_MALLOC);
	dprint(_MOD_ ": Adding redirection: (%s %s | doc:%s glob:%d exp:%d)\n",
		red->symbol, red->filename, red->heredoc_body, red->do_globbing,
		red->do_expansion);
	ft_lstadd_back(p_get_redirs_ptr(node), new);
	p_update_redc(node, 1);
	return (0);
}

// Stores redirection symbol, globbing, expansion, filename, and advances parser
/* tok: current redirect token returned from advance()
 * Freeing handled by caller in case of error
 */
static int	_process_normal_redir(t_parser *p, t_tok *tok, t_redir_data *red,
		t_ast_node *n)
{
	t_tok	*tok_fname;

	if (!p || !red || !tok)
		return (ERR_ARGS);
	red->symbol = ft_strdup_tmp(p->mmgr, tok_get_raw((t_tok *)tok));
	if (!red->symbol)
		exit_clean(&p->mmgr->list, ENOMEM, __FUNCTION__, EMSG_REDIR_SYM_MALLOC);
	dprint("%s: %s: redir typ:%d sym:%s\n", _MOD_, __FUNCTION__, red->type, red->symbol);
	if (is_at_end(p))
	{
		print_parse_error(p->global_state,
			tok_get_raw((p->curr_tok)->prev->content),
			tok_get_pos((p->curr_tok)->content));
		return (err(EMSG_REDIR_FN_EOF), ERR_SYNTAX);
	}
	tok_fname = advance(p);
	dprint("%s: %s: fname:%s_\n", _MOD_, __FUNCTION__, tok_get_raw(tok_fname));
	if (!(is_filename_token((t_tok *)tok_fname)
			|| is_expansion((t_tok *)tok_fname))
		|| 0 == ft_strcmp(tok_get_raw(tok_fname), ""))
	{
		print_redir_error(p->global_state,
			tok_get_raw((p->curr_tok)->prev->prev->content),
			tok_get_pos((p->curr_tok)->prev->prev->content));
		return (err(EMSG_REDIR_FN_INVALID), ERR_SYNTAX);
	}
	if (is_group_token(tok_fname))
	{
		dprint(_MOD_ ": %s: got group redir %p, node %p\n", __FUNCTION__,
			tok_get_tlist(tok_fname), n);
		red->is_groupred = true;
		p_set_has_redgrouptoks(n, 1);
		red->lst_tokens = ft_lstcopy_tmp(get_mem(p->global_state),
				tok_get_tlist(tok_fname), copy_token, destroy_token);
	}
	red->do_globbing = tok_get_globbing((t_tok *)tok_fname);
	red->do_expansion = tok_get_expansion((t_tok *)tok_fname);
	red->filename = ft_strdup_tmp(p->mmgr, tok_get_raw((t_tok *)tok_fname));
	red->target_ptr = n;
	p_set_do_redir_expansion(n, red->do_expansion);
	p_set_do_redir_globbing(n, red->do_globbing);
	if (!red->filename)
		exit_clean(&p->mmgr->list, ENOMEM, __FUNCTION__, EMSG_REDIR_FN);
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
	red->heredoc_body = ft_strdup_tmp(p->mmgr, tok_get_raw((t_tok *)tok));
	if (!red->heredoc_body)
		exit_clean(&p->mmgr->list, ENOMEM, __FUNCTION__, EMSG_HEREDOC_MALLOC);
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
			if (0 != _process_normal_redir(p, tok, red, node))
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
