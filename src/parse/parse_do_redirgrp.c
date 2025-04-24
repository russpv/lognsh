
#include "parse_int.h"

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
	if (ft_lstsize(*get_tmp_tok_list(s)) > 1)
		return (print_parse_redir_error(s, orig_pos), ERR_REDIR);
	res = do_redir_inserts(s, this_red);
	dprint("%s: returning lst_size: %d, %p\n", __FUNCTION__,
		ft_lstsize(*this_red), *this_red);
	return (res);
}
