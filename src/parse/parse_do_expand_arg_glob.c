#include "parse_int.h"

#define DBGMSG_GOTARGS _MOD_ ": p_do_globbing_args - arg: %s, lst:%p\n"
#define DBGMSG_MATCHES _MOD_ ": p_do_globbing_args found %d matches, 1st: %s\n"

/*
 * ins_lst is incoming list of glob match strings to be inserted
 * at the lst_node position, overwriting it.
 * content is the existing arg node.
 * Deletes lst_node, deep copies glst and inserts
 * at lst_node position.
 * glst destroyed in arg_destroy
 */
int	do_arg_inserts(t_mem_mgr *mgr, t_list **lst_node, t_list **ins_lst, t_arg_data *content)
{
	t_list	*new_arg_data_lst;
	char	*new_arg;

	new_arg_data_lst = NULL;
	new_arg = NULL;
	if (ft_lstsize(*ins_lst) > 1)
	{
		ft_lstdelone_rwd_tmp(mgr, lst_node, lst_node, destroy_arg);
		new_arg_data_lst = ft_lstmap_tmp(mgr, *ins_lst, create_arg_data_node_deep,
				destroy_arg);
		ft_lstadd_insert(lst_node, new_arg_data_lst);
		ft_lstclear_str_tmp(mgr, ins_lst);
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
	debug_print(DBGMSG_GOTARGS, arg->raw, (void *)lst_node);
	if (true == arg->do_globbing)
	{
		lst = match_glob(mgr, arg->raw);
		if (lst)
		{
			debug_print(DBGMSG_MATCHES, ft_lstsize(lst), lst->content);
			res = do_arg_inserts(mgr, lst_node, &lst, arg);
			if (0 != res)
				return (res);
		}
	}
	return (0);
}

