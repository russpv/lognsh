#include "parse_int.h"

#define EMSG_REDIR_HANDLER "Redirection handler issue\n"
#define EMSG_REDIR_NULLNODE "ERR no node given\n"
#define DM_R_G "%s: p_do_redir_processing got: type:%d glob_%d exp_%d grp_%d\n"
#define NOFN "No filename for input redirection.\n"

extern int	redirect(int *to, char *topath, int from, bool ifappend);

/* Strategy handlers for _p_do_redirection */

int	handle_redirect_in(const t_redir_data *node)
{
	const int	append = false;

	if (!node->filename)
		return (err(NOFN), ERR_ARGS);
	if (redirect(NULL, node->filename, STDIN_FILENO, append) < 0)
		return (err("Input redirection issue\n"), ERR_REDIR);
	return (0);
}

int	handle_heredoc(const t_redir_data *node)
{
	const int	append = false;
	int			fildes[2];

	if (!node->heredoc_body)
		return (err("No heredoc for input redirection.\n"), ERR_ARGS);
	if (pipe(fildes) < 0)
		return (perror(EMSG_PIPE), ERR_PIPE);
	write(fildes[1], node->heredoc_body, ft_strnlen(node->heredoc_body,
			MAX_OUTPUT_SZ));
	if (0 != close(fildes[1]))
		return (perror(EMSG_CLOSE), ERR_CLOSE);
	if (redirect(&fildes[0], NULL, STDIN_FILENO, append) < 0)
		return (err("Heredoc redirection issue\n"), ERR_REDIR);
	return (0);
}

int	handle_redirect_out(const t_redir_data *node)
{
	const int	append = false;

	if (!node->filename)
		return (err(NOFN), ERR_ARGS);
	if (redirect(NULL, node->filename, STDOUT_FILENO, append) < 0)
		return (err("Output redirection issue\n"), ERR_REDIR);
	return (0);
}

int	handle_redirect_append(const t_redir_data *node)
{
	const int	append = true;

	if (!node->filename)
		return (err(NOFN), ERR_ARGS);
	if (redirect(NULL, node->filename, STDOUT_FILENO, append) < 0)
		return (err("Append redirection issue\n"), ERR_REDIR);
	return (0);
}

/* Executes redirection of t_redir_data llist
 * Accepts a t_redir_data.
 * Assumes a non-null set of redirections exists.
 * Note: parser does not node-rize TOK_HEREDOC
 */
static int	_p_do_redirection(void *content)
{
	const t_redir_data	*redir = (t_redir_data *)content;
	const t_redir_fn	handlers[] = {[TOK_REDIRECT_IN] = handle_redirect_in,
	[TOK_REDIRECT_OUT] = handle_redirect_out,
	[TOK_REDIRECT_APPEND] = handle_redirect_append,
	[TOK_HEREDOC_WORD] = handle_heredoc};

	if (NULL == content)
		return (ERR_ARGS);
	if (!redir->type)
		return (dprint("%s: %s: got null\n", _MOD_, __FUNCTION__), ERR_ARGS);
	dprint("%s: %s: iterating...\n", _MOD_, __FUNCTION__);
	if (handlers[redir->type])
	{
		if (0 != handlers[redir->type](redir))
			return (err(EMSG_REDIR_HANDLER), ERR_REDIR);
		if (EACCES == errno)
			set_exit_status(redir->global_state, ERR_GENERAL);
	}
	else
		return (err(EMSG_REDIR_HANDLER), ERR_REDIR);
	return (0);
}

/* Executes redirections of a node in order.
 * If no redirections to do, returns ...
 * Expansions and globbing are to have been done earlier.
 * If error during redirection due to...
 * 	- multiple filenames, aborts command only
 *  - nonexistent input file, aborts command only
 *  - nonexistent output file, creates file and proceeds (no error)
 *  - output is a dir, aborts command only
 */
int	p_do_redirections(t_ast_node *a)
{
	if (!a)
		return (err(EMSG_REDIR_NULLNODE), ERR_ARGS);
	if (a->type != AST_NODE_CMD && a->type != AST_NODE_PROC)
		return (err("Redirs: invalid cmd type\n"), EINVAL);
	if (false == node_has_redirects(a))
		return (0);
	dprint("%s: %s: doing redirections...\n", _MOD_, __FUNCTION__);
	if (a->type == AST_NODE_CMD)
		if (0 != ft_lstiter(a->data.cmd.redirs, _p_do_redirection))
			return (ERR_REDIR);
	if (a->type == AST_NODE_PROC)
		if (0 != ft_lstiter(a->data.proc.redirs, _p_do_redirection))
			return (ERR_REDIR);
	return (0);
}

static void	_dprint(t_ast_node *a)
{
	dprint(DM_R_G, _MOD_, p_get_type(a),
		p_get_do_redir_glob(a), p_get_do_redir_exp(a),
		p_get_has_redgrptok(a));
}

static int	_do_normal(t_state *s, t_ast_node *a, t_list **redirs)
{
	char	*orig_filen;
	int		res;

	res = 0;
	orig_filen = NULL;
	if (true == p_get_do_redir_exp(a))
		res = lstiter_state(s, *redirs, p_do_red_expansion);
	if (0 != res)
		return (res);
	if (true == p_get_do_redir_glob(a))
		orig_filen = ft_lstiterstr_mem(get_mem(s), *redirs,
				p_do_globbing_redirs);
	if (NULL != orig_filen)
	{
		print_ambig_redir(((t_redir_data *)orig_filen)->filename);
		return (ERR_AMBIGUOUS_REDIR);
	}
	return (0);
}

/* Modifies redirection list in case of expansions.
 */
int	p_do_redir_processing(t_state *s, t_ast_node *a)
{
	t_list **const	redirs = p_get_redirs_ptr(a);
	int				res;

	if (a->type != AST_NODE_CMD && a->type != AST_NODE_PROC)
		return (err("Invalid node type\n"), ERR_ARGS);
	if (redirs)
	{
		_dprint(a);
		if (true == p_get_has_redgrptok(a))
		{
			res = ft_lstiter_state_ins_rwd_mem(s, redirs,
					p_do_grpred_processing);
			if (0 != res)
				return (res);
		}
		else
		{
			res = _do_normal(s, a, redirs);
			if (0 != res)
				return (res);
		}
		dprint("%s: %s: done.\n", _MOD_, __FUNCTION__);
	}
	return (0);
}


/* -------------------------------------------------------------------------
 * Function Group: Group Token Redirection Logic
 * Description   : Handles dispatching of group token redirections
 *                 during command execution.
 * ------------------------------------------------------------------------- */

// Can only replace/update the grp node with the combined word
int	do_redir_inserts(t_state *s, t_list **this_node)
{
	t_list			**tok_lst;
	t_redir_data	*r;

	tok_lst = get_tmp_tok_list(s);
	r = (*this_node)->content;
	r->filename = tok_get_raw((*tok_lst)->content);
	ft_lstprinter(*this_node, print_redir_tok);
	*tok_lst = NULL;
	lgprint("Redir inserts done.\n");
	return (0);
}

// Modifies token list
static int	_do_expansions(t_state *s, t_redir_data *grpred, int *res)
{
	if (!grpred || !s)
	{
		*res = ERR_ARGS;
		return (ERR_ARGS);
	}
	if (grpred->do_expansion)
	{
		*res = lstiter_state(s, grpred->lst_tokens, tok_do_expansion);
		if (0 != *res)
			return (*res);
		tok_print_list(grpred->lst_tokens);
	}
	lgprint("Redir expansions done; lstsz:%d.\n",
		ft_lstsize(grpred->lst_tokens));
	return (*res);
}

static int	_do_wordsplits(t_state *s, t_redir_data *grpred, int *res)
{
	if (!grpred || !s)
	{
		*res = ERR_ARGS;
		return (ERR_ARGS);
	}
	if (grpred->do_expansion && !grpred->in_dquotes)
		ft_lstiter_ins_rwd_tmp(get_mem(s), &grpred->lst_tokens,
			tok_do_wordsplits);
	tok_print_list(grpred->lst_tokens);
	lgprint("Redir wordsplits done; lstsz:%d.\n",
		ft_lstsize(grpred->lst_tokens));
	return (*res);
}

static int	_do_globbing(t_state *s, t_redir_data *grpred, int *res)
{
	t_list	**saved_lst;

	*res = 0;
	if (!grpred || !s)
	{
		*res = ERR_ARGS;
		return (ERR_ARGS);
	}
	saved_lst = get_tmp_tok_list(s);
	if (grpred->do_globbing || grpred->do_expansion)
	{
		ft_lstiter_ins_rwd_tmp(get_mem(s), saved_lst, p_do_globbing_toks);
	}
	lgprint("Redir globbing done.\n");
	return (*res);
}

// Saves modified token list in state cache
static int	_do_combine(t_state *s, t_redir_data *grpred, int *res)
{
	char	*str;
	t_list	**tok_lst;

	*res = lstiter_state(s, grpred->lst_tokens, tok_do_grp_combine);
	if (0 != *res)
		return (err("grp tok aggregation failure\n"), *res);
	tok_lst = get_tmp_tok_list(s);
	str = get_tmp(s);
	if (str)
	{
		if (*str)
			ft_lstadd_back(tok_lst, ft_lstnew_tmp(get_mem(s),
					create_tmp_token(get_mem(s), str)));
	}
	tok_print_list(*tok_lst);
	lgprint("Redir combines done; lstsz:%d.\n",
		ft_lstsize(*get_tmp_tok_list(s)));
	return (*res);
}

// this_red: list of t_redir_data
// if not single ll node on the tmp ptr, errors
int	p_do_grpred_processing(t_state *s, t_list **this_red, void *c)
{
	t_redir_data	*grpred;
	int				res;
	size_t			orig_pos;

	grpred = (t_redir_data *)c;
	res = 0;
	if (NULL == c || false == grpred->is_groupred || NULL == grpred->lst_tokens)
		return (0);
	dprint(_MOD_ ": %s: got lst: %p exp:%d glob:%d\n", __FUNCTION__,
		grpred->lst_tokens, grpred->do_expansion, grpred->do_globbing);
	orig_pos = tok_get_pos(ft_lstlast(grpred->lst_tokens)->content);
	if (0 != _do_expansions(s, grpred, &res))
		return (res);
	if (0 != _do_wordsplits(s, grpred, &res))
		return (res);
	if (0 != _do_combine(s, grpred, &res))
		return (res);
	if (0 != _do_globbing(s, grpred, &res))
		return (res);
	if (ft_lstsize(*get_tmp_tok_list(s)) != 1)
		return (print_parse_redir_error(s, orig_pos), ERR_REDIR);
	res = do_redir_inserts(s, this_red);
	dprint("%s: returning lst_size: %d, %p\n", __FUNCTION__,
		ft_lstsize(*this_red), *this_red);
	return (res);
}
