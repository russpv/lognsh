#include "parse_int.h"

t_parse_fns	*init_parse_fns(t_state *s)
{
	const t_mem_mgr	*m = get_mem(s);
	t_parse_fns		*fns;

	fns = m->f(&((t_mem_mgr *)m)->list, sizeof(t_parse_fns));
	if (!fns)
		exit_clean(&((t_mem_mgr *)m)->list, ENOMEM, __FUNCTION__, EMSG_MALLOC);
	fns->p_do_redirections = p_do_redirections;
	return (fns);
}

static inline void	_init_parser(t_state *s, t_parser *p)
{
	p->ast = NULL;
	p->curr_idx = -1;
	p->curr_cmd = NULL;
	p->last_node = NULL;
	p->ref_node = NULL;
	p->parse_error = false;
	p->global_state = s;
	p->mmgr = get_mem(s);
	register_parser_destroy(s, destroy_parser);
}

t_parser	*create_parser(t_state *s, t_list *tokens)
{
	t_parser	*p;

	if (!tokens)
		return (NULL);
	p = (t_parser *)get_mem(s)->f(&get_mem(s)->list, sizeof(t_parser));
	if (p)
	{
		p->tokens = ft_lstcopy_tmp(get_mem(s), tokens, copy_token,
				destroy_token);
		if (!p->tokens)
			exit_clean(&get_mem(s)->list, ENOMEM, __FUNCTION__, EMSG_MALLOC);
		p->curr_tok = tokens;
		p->token_count = ft_lstsize(tokens);
		dprint("p->tokc=%d\n", p->token_count);
		_init_parser(s, p);
		p->st = st_int_create(get_mem(s));
		if (!p->st)
			exit_clean(&get_mem(s)->list, ENOMEM, __FUNCTION__, EMSG_MALLOC);
	}
	return (p);
}

void	destroy_parser(t_mem_mgr *m, void **instance)
{
	t_parser	*p;

	p = (t_parser *)(*instance);
	if (!p)
		return ;
	dprint(_MOD_ ": %s...\n", __FUNCTION__);
	if (p->ast)
	{
		destroy_ast_node(m, (void **)&p->ast);
		p->ast = NULL;
	}
	if (p->tokens)
	{
		dprint(_MOD_ ": %s: destroying p->tokens...\n", __FUNCTION__);
		ft_lstclear_tmp(m, &p->tokens, destroy_token);
		p->tokens = NULL;
	}
	if (p->st)
	{
		st_int_destroy(m, p->st);
		p->st = NULL;
	}
	m->dealloc(&m->list, p);
	*instance = NULL;
}


/* -------------------------------------------------------------------------
 * Function Group: AST node data element inits
 * Description   : Handles creation/destruction of arg/redir list objects
 * ------------------------------------------------------------------------- */

#define EMSG_REDIR_DATA_MALLOC "Memory allocation failed for redirection data\n"

/* Fully frees a struct s_redir, for use in llist destroy */
void	destroy_redir(t_mem_mgr *m, void **in)
{
	t_redir_data	*redir;

	redir = (t_redir_data *)(*in);
	if (NULL == redir)
		return ;
	dprint(_MOD_ ": %s...\n", __FUNCTION__);
	if (redir->symbol)
		m->dealloc(&m->list, redir->symbol);
	if (redir->filename)
		m->dealloc(&m->list, redir->filename);
	if (redir->heredoc_body)
		m->dealloc(&m->list, redir->heredoc_body);
	if (redir->lst_glob)
		ft_lstclear_str_tmp(m, &redir->lst_glob);
	if (redir->lst_tokens)
		ft_lstclear_tmp(m, &redir->lst_tokens, destroy_token);
	m->dealloc(&m->list, redir);
	*in = NULL;
}

/* Fully frees a struct s_arg, for use in llist destroy */
void	destroy_arg(t_mem_mgr *mgr, void **in)
{
	t_arg_data	*arg;

	if (!in || !mgr)
		return ;
	arg = (t_arg_data *)(*in);
	dvprint(_MOD_ ":   %s...\n", __FUNCTION__);
	if (arg->lst_tokens)
		ft_lstclear_tmp(mgr, &arg->lst_tokens, destroy_token);
	if (arg->raw)
		mgr->dealloc(&mgr->list, arg->raw);
	if (arg->tmp)
		ft_freearr_mem(&mgr->list, mgr->dealloc, (void **)arg->tmp, -1);
	mgr->dealloc(&mgr->list, arg);
	*in = NULL;
}

// Called on redir tokens, which only have the symbol info.
t_redir_data	*init_redir(t_mem_mgr *m, t_parser *p, t_ast_node *target,
		enum e_tok_type type)
{
	t_redir_data	*red;

	if (!target)
		return (NULL);
	red = m->f(&m->list, sizeof(t_redir_data));
	if (!red)
		exit_clean(&m->list, ENOMEM, __FUNCTION__, EMSG_REDIR_DATA_MALLOC);
	red->type = type;
	red->target_ptr = target;
	red->do_globbing = false;
	red->do_expansion = false;
	red->in_dquotes = false;
	red->is_groupred = false;
	red->heredoc_body = NULL;
	red->symbol = NULL;
	red->filename = NULL;
	red->global_state = p->global_state;
	red->lst_glob = NULL;
	red->lst_tokens = NULL;
	return (red);
}

static void	_init_arg_helper(t_arg_data *arg, t_parser *p, t_tok *tok)
{
	arg->global_state = p->global_state;
	arg->tmp = NULL;
	arg->option = is_option(tok);
	arg->do_globbing = tok_get_globbing(tok);
	arg->do_expansion = tok_get_expansion(tok);
	arg->in_dquotes = tok_get_dquotes(tok);
	arg->is_grouparg = tok_isgrouptoken(tok);
}

/* Must deep copy token strings to decouple token-list/ast.
 */
t_arg_data	*init_arg(t_mem_mgr *m, t_parser *p, t_ast_node *cmd_node,
		t_tok *tok)
{
	t_arg_data	*arg;

	if (!p || !cmd_node || !tok)
		return (NULL);
	arg = m->f(&m->list, sizeof(struct s_arg));
	if (arg)
	{
		if (tok_get_raw(tok))
		{
			arg->raw = ft_strdup_mem(m, tok_get_raw(tok));
			if (!arg->raw)
				exit_clean(&m->list, ENOMEM, __FUNCTION__, EMSG_MALLOC);
		}
		else
			arg->raw = NULL;
		_init_arg_helper(arg, p, tok);
		arg->lst_tokens = ft_lstcopy_tmp(m, tok_get_tlist(tok), copy_token,
				destroy_token);
		if (false == arg->in_dquotes)
			cmd_node->data.cmd.do_wordsplit = true;
		cmd_node->data.cmd.do_expansion |= arg->do_expansion;
		cmd_node->data.cmd.do_globbing |= arg->do_globbing;
		cmd_node->data.cmd.has_arggrouptoks |= arg->is_grouparg;
	}
	return (arg);
}

/* t_arg_data llist copy constructor using a llist of char*.
 * Returns new void *content for llist construction/duplication.
 * Post processing, t_arg_data member values are no longer useful
 * Copies content. Ensures globbing pass is done.
 */
void	*create_arg_data_node_deep(t_mem_mgr *mgr, const void *content)
{
	t_arg_data	*arg_data;
	char		*raw;

	raw = (char *)content;
	arg_data = mgr->f(&mgr->list, sizeof(t_arg_data));
	if (arg_data)
	{
		arg_data->raw = ft_strdup_mem(mgr, raw);
		if (!arg_data->raw)
			exit_clean(&mgr->list, ENOMEM, __FUNCTION__, EMSG_MALLOC);
		arg_data->do_expansion = false;
		arg_data->do_globbing = false;
		arg_data->global_state = NULL;
		arg_data->in_dquotes = false;
		arg_data->option = false;
		arg_data->tmp = NULL;
		arg_data->is_grouparg = false;
		arg_data->lst_tokens = NULL;
	}
	return ((void *)arg_data);
}

/* t_arg_data llist copy constructor using a llist of char*.
 * Returns new void *content for llist construction/duplication.
 * Only the target filename is needed.
 * Inits t_redir_data dummy values.
 */
void	*create_redir_data_node_deep(t_mem_mgr *mgr, const void *content)
{
	t_redir_data	*red_data;
	char			*fn;

	fn = (char *)content;
	red_data = mgr->f(&mgr->list, sizeof(t_redir_data));
	if (red_data)
	{
		red_data->filename = ft_strdup_mem(mgr, fn);
		if (!red_data->filename)
			exit_clean(&mgr->list, ENOMEM, __FUNCTION__, EMSG_MALLOC);
		red_data->do_expansion = false;
		red_data->do_globbing = false;
		red_data->in_dquotes = false;
		red_data->is_groupred = false;
		red_data->lst_tokens = NULL;
		red_data->lst_glob = NULL;
		red_data->global_state = NULL;
		red_data->heredoc_body = NULL;
		red_data->symbol = NULL;
		red_data->target_ptr = NULL;
		red_data->type = 0;
	}
	return ((void *)red_data);
}


/* -------------------------------------------------------------------------
 * Function Group: AST node inits
 * Description   : Handles creation/destruction of node types
 * ------------------------------------------------------------------------- */

//mgr->dealloc(&mgr->list, node);
void	destroy_cmd_node(t_mem_mgr *mgr, void **n)
{
	t_ast_node	*node;

	if (NULL == n)
		return ;
	if (NULL == *n)
		return ;
	node = (t_ast_node *)(*n);
	dvprint(_MOD_ ":  destroy_cmd_node...\n");
	if (NULL == node)
		return ;
	if (node->type != AST_NODE_CMD)
		return ;
	if (node->data.cmd.name)
		mgr->dealloc(&mgr->list, node->data.cmd.name);
	if (node->data.cmd.args)
		ft_lstclear_tmp(mgr, &node->data.cmd.args, destroy_arg);
	if (node->data.cmd.redirs)
		ft_lstclear_tmp(mgr, &node->data.cmd.redirs, destroy_redir);
	dvprint(_MOD_ ":  destroy_cmd_node...DONE\n");
	*n = NULL;
}

//mgr->dealloc(&mgr->list, node);
void	destroy_proc_node(t_mem_mgr *mgr, void **n)
{
	t_ast_node	*node;

	if (NULL == n)
		return ;
	if (NULL == *n)
		return ;
	node = (t_ast_node *)(*n);
	dvprint(_MOD_ ":  destroy_proc_node...\n");
	if (NULL == node)
		return ;
	if (AST_NODE_PROC != node->type)
		return ;
	if (node->data.proc.redirs)
		ft_lstclear_tmp(mgr, &node->data.proc.redirs, destroy_redir);
	if (node->data.proc.cmds)
		ft_lstclear_tmp(mgr, &node->data.proc.cmds, destroy_ast_node);
	dvprint(_MOD_ ":  destroy_proc_node...DONE\n");
	*n = NULL;
}

// destroy t_list cmds, char **operators
//mgr->dealloc(&mgr->list, node);
void	destroy_log_node(t_mem_mgr *mgr, void **n)
{
	t_ast_node	*node;

	if (NULL == n)
		return ;
	if (NULL == *n)
		return ;
	node = (t_ast_node *)(*n);
	dvprint(_MOD_ ":  destroy_log_node...\n");
	if (NULL == node)
		return ;
	if (AST_NODE_LOG != node->type)
		return ;
	if (node->data.log.operators)
		ft_lstclear_str_tmp(mgr, &node->data.log.operators);
	if (node->data.log.cmds)
		ft_lstclear_tmp(mgr, &node->data.log.cmds, destroy_ast_node);
	dvprint(_MOD_ ":  destroy_log_node...DONE\n");
	*n = NULL;
}

//mgr->dealloc(&mgr->list, node);
void	destroy_pipe_node(t_mem_mgr *mgr, void **n)
{
	t_ast_node	*node;

	if (NULL == n)
		return ;
	if (NULL == *n)
		return ;
	node = (t_ast_node *)(*n);
	dvprint(_MOD_ ":  destroy_pipe_node...\n");
	if (NULL == node)
		return ;
	if (AST_NODE_PIPELINE != node->type)
		return ;
	if (node->data.pipe.cmds)
		ft_lstclear_tmp(mgr, &node->data.pipe.cmds, destroy_ast_node);
	dvprint(_MOD_ ":  destroy_pipe_node...DONE\n");
	*n = NULL;
}

/* Switch for freeing various t_ast_node types
 * For use in llist destroy
 */
void	destroy_ast_node(t_mem_mgr *mgr, void **node)
{
	t_ast_node	*ast;

	if (NULL == node)
		return ;
	if (NULL == *node)
		return ;
	ast = (t_ast_node *)(*node);
	dvprint(_MOD_ ":  destroy_ast...\n");
	if ((uintptr_t)ast < 0x1000)
		return ;
	if (NONE == ast->type)
		return ;
	else if (AST_NODE_PROC == ast->type)
		destroy_proc_node(mgr, node);
	else if (AST_NODE_CMD == ast->type)
		destroy_cmd_node(mgr, node);
	else if (AST_NODE_PIPELINE == ast->type)
		destroy_pipe_node(mgr, node);
	else if (AST_NODE_LOG == ast->type)
		destroy_log_node(mgr, node);
	dvprint(_MOD_ ":  destroy_ast...DONE\n");
}
