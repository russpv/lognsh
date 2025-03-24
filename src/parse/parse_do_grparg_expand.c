#include "parse_int.h"

static int	_do_expansions(t_state *s, t_arg_data *grparg)
{
	int res;

	if (!grparg || !s)
		return (ERR_ARGS);
	if (grparg->do_expansion)
	{
		res = lstiter_state(s, grparg->lst_tokens, tok_do_expansion);
		if (0 != res)
			return (res);
	}
	tok_print_list(grparg->lst_tokens);
	return (0);
}

static int	_do_globbing(t_state *s, t_arg_data *grparg)
{
	t_list		**lst; //grparg's token list

	if (!grparg || !s)
		return (ERR_ARGS);
	if (grparg->do_globbing)
	{
		lst = &grparg->lst_tokens;
		ft_lstiter_ins_rwd_tmp(get_mem(s), lst, p_do_globbing_args);
	}
	return (0);
}

// TODO, mark which ones can't combine
static int	_do_wordsplits(t_state *s, t_arg_data *grparg)
{
	if (!grparg || !s)
		return (ERR_ARGS);
	if (grparg->do_expansion && !grparg->in_dquotes)
		ft_lstiter_ins_rwd_tmp(get_mem(s), &grparg->lst_tokens, tok_do_wordsplits); 
	tok_print_list(grparg->lst_tokens);
	return (0);
}

void _print_arg(void *arg)
{
	t_arg_data *node;

	node = (t_arg_data *)arg;
	fprintf(stderr, "%s", node->raw);
}

/* Replace this group arg node with resulting promoted args based on token list */
static int	_do_combine_and_insert(t_state *s, t_arg_data *grparg, t_list **this_node)
{
	int res;
	char *str;
	t_list *tok_lst;

	res = lstiter_state(s, grparg->lst_tokens, tok_do_grp_combine);
	if (0 != res)
		return (err("grp tok aggregation failure\n"), res);
	tok_lst = get_tmp_tok_list(s);
	str = get_tmp(s);
	if (str)
	{
		fprintf(stderr, "got tmp:%s\n",str);
		ft_lstadd_back(&tok_lst, ft_lstnew_tmp(get_mem(s), create_tmp_token(get_mem(s), str)));
	}
	tok_print_list(tok_lst);
	fprintf(stderr, "printed combined list\n");

	t_list *argl = ft_lstmap_tmp(get_mem(s), tok_lst, token_to_arg, destroy_arg);
	ft_lstprinter(argl, _print_arg);
	ft_lstadd_insert(this_node, argl); 
	ft_lstprinter(*this_node, _print_arg);
	ft_lstdelone_tmp(get_mem(s), this_node, *this_node, destroy_arg);
	ft_lstprinter(*this_node, _print_arg);
	return (0);
}

/* Passed to arg llist iterator to iterate any group arg's token llist.
 * If (null) results, still returns empty heap string,
 * since a grouparg implies non-null word.
 * Ignores all (null) token raws.
 * Inserts into arg llist as needed. 
 */
int	p_do_grparg_processing(t_state *s, t_list **this, void *c)
{
	t_arg_data	*grparg;
	int			res;

	grparg = (t_arg_data *)c;
	if (NULL == c || false == grparg->is_grouparg || NULL == grparg->lst_tokens)
		return (0);
	debug_print(_MOD_ ": %s: got list: %p exp:%d glob:%d\n", __FUNCTION__,\
		grparg->lst_tokens, grparg->do_expansion, grparg->do_globbing);
	res = _do_expansions(s, grparg);
	if (0 != res)
		return res;
	res = _do_wordsplits(s, grparg);
	if (0 != res)
		return res;
	res = _do_globbing(s, grparg);
	if (0 != res)
		return res;
	res = _do_combine_and_insert(s, grparg, this);
	debug_print("%s: returning lst_size: %d, %p\n", __FUNCTION__, ft_lstsize(*this), *this);
	return (res);
}
