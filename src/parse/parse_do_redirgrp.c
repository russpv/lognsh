#include "parse_int.h"

// Modifies args token list
static int	_do_expansions(t_state *s, t_redir_data *grpred)
{
	int res;

	if (!grpred || !s)
		return (ERR_ARGS);
	if (grpred->do_expansion)
	{
		res = lstiter_state(s, grpred->lst_tokens, tok_do_expansion);
		if (0 != res)
			return (res);
		tok_print_list(grpred->lst_tokens);
	}
	return (0);
}

static int	_do_wordsplits(t_state *s, t_redir_data *grpred)
{
	if (!grpred || !s)
		return (ERR_ARGS);
	if (grpred->do_expansion && !grpred->in_dquotes)
		ft_lstiter_ins_rwd_tmp(get_mem(s), &grpred->lst_tokens, tok_do_wordsplits);
	tok_print_list(grpred->lst_tokens);
	return (0);
}

static int	_do_globbing(t_state *s, t_redir_data *grpred)
{
	t_list		**saved_lst;

	if (!grpred || !s)
		return (ERR_ARGS);
	saved_lst = get_tmp_tok_list(s);
	if (grpred->do_globbing || grpred->do_expansion)
	{
		ft_lstiter_ins_rwd_tmp(get_mem(s), saved_lst, p_do_globbing_toks);
	}
	return (0);
}

int _print_redir(void *redir)
{
	t_redir_data *node;

	node = (t_redir_data *)redir;
	if (node)
	{
		if (node->symbol)
			colored_printf(MAGENTA, "%s", node->symbol);
		else
			colored_printf(MAGENTA, "(null)");
	}
	return (0);
}

//Saves modified token list in state cache
static int	_do_combine(t_state *s, t_redir_data *grpred)
{
	int res;
	char *str;
	t_list **tok_lst;

	res = lstiter_state(s, grpred->lst_tokens, tok_do_grp_combine);
	if (0 != res)
		return (err("grp tok aggregation failure\n"), res);
	tok_lst = get_tmp_tok_list(s);
	str = get_tmp(s);
	if (str)
	{
		if (*str)
			ft_lstadd_back(tok_lst, ft_lstnew_tmp(get_mem(s), create_tmp_token(get_mem(s), str)));
	}
	tok_print_list(*tok_lst);
	return (0);
}

static void	_do_insert(t_state *s, t_list **this_node)
{
	t_list **tok_lst;

	tok_lst = get_tmp_tok_list(s);
	t_list *argl = ft_lstmap_tmp(get_mem(s), *tok_lst, token_to_arg, destroy_arg);
	ft_lstadd_insert(this_node, argl);
	ft_lstdelone_tmp(get_mem(s), this_node, *this_node, destroy_arg);
	ft_lstprinter(*this_node, _print_redir);
	*tok_lst = NULL;
}

int	p_do_grpred_processing(t_state *s, t_list **this_red, void *c)
{
	t_redir_data	*grpred;
	int			res;

	grpred = (t_redir_data *)c;
	if (NULL == c || false == grpred->is_groupred || NULL == grpred->lst_tokens)
		return (0);
	debug_print(_MOD_ ": %s: got list: %p exp:%d glob:%d\n", __FUNCTION__,\
		grpred->lst_tokens, grpred->do_expansion, grpred->do_globbing);
	res = _do_expansions(s, grpred);
	if (0 != res)
		return res;
	log_print("Expansions Done.\n");
	res = _do_wordsplits(s, grpred);
	if (0 != res)
		return res;
	log_print("Wordsplits Done.\n");
	res = _do_combine(s, grpred);
	if (0 != res)
		return res;
	log_print("Combines and Inserts Done.\n");
	res = _do_globbing(s, grpred);
	if (0 != res)
		return res;
	_do_insert(s, this_red);
	log_print("Globbing Done.\n");
	debug_print("%s: returning lst_size: %d, %p\n", __FUNCTION__, ft_lstsize(*this_red), *this_red);
	return (res);
}
