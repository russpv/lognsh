#include "parse_int.h"

// Splits words and inserts as args, replacing lst_node.
// Explicit empty strings retained as empty strings.
// Unquoted nulls resulting from expansions are removed.
// Quoted nulls resulting from expansions are retained as empty strings, (done prior)
// unless part of a non-null expansion word. -d'' is -d
// Inserts the list at lst_node position
int	p_do_wordsplits(t_mem_mgr *mgr, t_list **lst_node, void *lst_c)
{
	t_arg_data	*arg;
	t_list		*str_lst;
	int			res;

	res = 0;
	str_lst = NULL;
	arg = (t_arg_data *)lst_c;
	if (false == arg->in_dquotes && true == arg->do_expansion)
	{
		debug_print(_MOD_ ": %s: got _%s_ \n", __FUNCTION__, arg->raw);
		str_lst = split_word(mgr, arg->raw);
		if (str_lst)
		{
			if (str_lst->content)
				debug_print("%d %s\n", ft_lstsize(str_lst), str_lst->content);
			res = do_arg_inserts(mgr, lst_node, &str_lst, arg);
			if (0 != res)
				return (res);
		}
		debug_print(_MOD_ ": %s: done. \n", __FUNCTION__);
	}
	return (0);
}
