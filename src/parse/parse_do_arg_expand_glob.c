
#include "parse_int.h"

#define DBGMSG_GOTARGS "Parse: %s - arg: %s, lst:%p\n"
#define DM_MS "Parse: %s found %d matches, 1st: %s\n"

// Tests whether globstar is preceded with escape char
static bool	_has_globbing(const char *raw)
{
	char	*globstar;

	globstar = ft_strchr(raw, '*');
	if (NULL == globstar)
		return (false);
	if (globstar != raw)
		if ('\\' == *(globstar - 1))
			return (false);
	return (true);
}

// Gets globbing matches and inserts tokens. Used with list iterator.
int	p_do_globbing_toks(t_mem_mgr *mgr, t_list **lst_node, void *lst_c)
{
	t_tok	*tok;
	t_list	*lst;
	int		res;

	res = 0;
	lst = NULL;
	if (!lst_c)
		return (ERR_ARGS);
	tok = (t_tok *)lst_c;
	if (!tok_get_raw(tok))
		return (ERR_ARGS);
	dvprint(DBGMSG_GOTARGS, _MOD_, __FUNCTION__, tok_get_raw(tok),
		(void *)lst_node);
	if (true == _has_globbing(tok_get_raw(tok)))
	{
		lst = match_glob(mgr, tok_get_raw(tok));
		if (lst)
		{
			dprint(DM_MS, __FUNCTION__, ft_lstsize(lst), lst->content);
			res = do_tok_inserts(mgr, lst_node, &lst, create_tmp_token);
			if (0 != res)
				return (res);
		}
	}
	return (0);
}

static void	_do_stuff(t_mem_mgr *mgr, t_list **lst_node, t_list **ins_lst)
{
	t_list	*new_arg_data_lst;
	bool	reattach_head;
	t_list	*head;

	new_arg_data_lst = NULL;
	reattach_head = false;
	new_arg_data_lst = ft_lstmap_tmp(mgr, *ins_lst, create_arg_data_node_deep,
			destroy_arg);
	ft_lstadd_insert(lst_node, new_arg_data_lst);
	head = ft_lstfirst(*lst_node);
	if (head == *lst_node)
		reattach_head = true;
	ft_lstdelone_rwd_tmp(mgr, &head, lst_node, destroy_arg);
	if (reattach_head)
		*lst_node = head;
	ft_lstclear_str_tmp(mgr, ins_lst);
}

/*
 * ins_lst is incoming list of glob match strings to be inserted
 * at the lst_node position, overwriting it.
 * content is the existing arg node.
 * Deletes lst_node, deep copies glst and inserts
 * at lst_node position.
 * glst destroyed in arg_destroy
 */
int	do_arg_inserts(t_mem_mgr *mgr, t_list **lst_node, t_list **ins_lst,
		t_arg_data *content)
{
	char	*new_arg;

	new_arg = NULL;
	if (ft_lstsize(*ins_lst) > 1)
	{
		_do_stuff(mgr, lst_node, ins_lst);
	}
	else
	{
		new_arg = ft_strdup_tmp(mgr, (*ins_lst)->content);
		if (!new_arg)
			exit_clean(&mgr->list, ENOMEM, __FUNCTION__, EMSG_MALLOC);
		mgr->dealloc(&mgr->list, content->raw);
		content->raw = new_arg;
		ft_lstclear_str_tmp(mgr, ins_lst);
	}
	return (0);
}

/* DO GLOBBING
 *
 * Called within t_list reverse iterator
 *  to handle insertions (in args lists)
 * Modifies argument t_list. Called during
 * execution, as globbing is done after parsing.
 *
 * Returns early in case of ambiguous redirect.
 *
 * Arg lst_c: lst_node's t_redir_data *content
 * Arg lst_node: node
 *
 * Error msg is "[filename]: no such file or directory\n"
 *
 * If multiple words found by glob, returns error:
 * "[original string]: ambiguous redirect\n"
 *
 * Anything returned from globbing matches will
 * NOT be processed further.
 */
int	p_do_globbing_args(t_mem_mgr *mgr, t_list **lst_node, void *lst_c)
{
	t_arg_data	*arg;
	t_list		*lst;
	int			res;

	res = 0;
	lst = NULL;
	if (!lst_c)
		return (ERR_ARGS);
	arg = (t_arg_data *)lst_c;
	if (!arg->raw)
		return (ERR_ARGS);
	dvprint(DBGMSG_GOTARGS, __FUNCTION__, arg->raw, (void *)lst_node);
	if (true == arg->do_globbing || true == _has_globbing(arg->raw))
	{
		lst = match_glob(mgr, arg->raw);
		if (lst)
		{
			dprint(DM_MS, __FUNCTION__, ft_lstsize(lst), lst->content);
			res = do_arg_inserts(mgr, lst_node, &lst, arg);
			if (0 != res)
				return (res);
		}
	}
	return (0);
}
