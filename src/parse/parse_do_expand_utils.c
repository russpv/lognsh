#include "parse_int.h"

/* Custom llist iterator with t_state function ptr
 * Calls f with NULL once after all nodes iterated
 */
int	lstiter_state(t_state *s, t_list *lst, int (*f)(t_state *, void *))
{
	int	res;

	res = 0;
	if (lst == NULL)
		return (ERR_ARGS);
	while (lst)
	{
		res = f(s, (lst)->content);
		if (0 != res)
			return (ERR_GENERAL);
		lst = (lst)->next;
	}
	f(s, NULL);
	return (0);
}

/* Iterates BACKWARDS and applies insertion func f */
int	ft_lstiter_state_ins_rwd_tmp(t_state *s, t_list **lst,
	int (*f)(t_state *, t_list **, void *))
{
	t_list	*lst_rear;
	t_list	*tmp;

	if (lst == NULL)
		return (debug_print("Null arg\n", __FUNCTION__), ERR_ARGS);
	lst_rear = ft_lstlast(*lst);
	debugv_print(_MOD_ ": %s: %p\n", __FUNCTION__, (void *)lst_rear);
	while (lst_rear)
	{
		tmp = lst_rear->prev;
		debugv_print(_MOD_ "%s: %p, content:%p\n", __FUNCTION__, (void *)lst_rear,
			(void *)lst_rear->content);
		f(s, &(lst_rear), lst_rear->content);
		debugv_print(_MOD_ "%s: Applied f to %p\n", __FUNCTION__, (void *)lst_rear);
		if (NULL == tmp)
			*lst = lst_rear;
		lst_rear = tmp;
	}
	debug_print(_MOD_ ": %s: Inserts done successfully.\n", __FUNCTION__);
	return (0);
}

// Returns the args removed as a negative qty
int	lstiter_state_rwd_trim(t_state *s, t_list **lst, int (*test)(void *),
		void (*del)(t_mem_mgr *, void **))
{
	int			res;
	t_list		*lst_rear;
	t_list		*tmp;
	int			args_removed;

	args_removed = 0;
	if (lst == NULL)
		return (ERR_ARGS);
	lst_rear = ft_lstlast(*lst);
	while (lst_rear)
	{
		res = 0;
		tmp = (lst_rear)->prev;
		res = test((lst_rear)->content);
		if (res > 0)
		{
			ft_lstdelone_rwd_tmp(get_mem(s), lst, &lst_rear, del);
			args_removed--;
		}
		if (res < 0)
			return (ERR_GENERAL);
		lst_rear = tmp;
	}
	return (args_removed);
}

// Called on ist->content; requires tok to be a NORMAL tok
void	*token_to_arg(t_mem_mgr *m, const void *tok)
{
	if (!tok)
		return (NULL);
	return (create_arg_data_node_deep(m, tok_get_raw((t_tok *)tok)));
}

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

int	p_is_arg_null(void *c)
{
	t_arg_data	*content;

	content = (t_arg_data *)c;
	if (NULL == c)
		return (-1);
	if (true == content->is_grouparg)
		return (0);
	if (NULL == content->raw)
		return (1);
	return (0);
}

/* Add special $? codes here.
 * buf will start with char after any '$'.
 * Returns the negated length of the expansion found (minus $).
 * Or error codes.
 * Places the expanded heap string in *value
 */
int	check_special_expansions(t_state *s, const char *buf, char **value)
{
	const int	*status = get_status(s);

	if (!buf || !value)
		return (ERR_ARGS);
	debug_print(DMSG_IN, __FUNCTION__, buf);
	if (ft_strcmp(buf, "?") == 0)
	{
		if (status == NULL)
			*value = ft_itoa_mem(&get_mem(s)->list, myalloc, 0);
		else
			*value = ft_itoa_mem(&get_mem(s)->list, myalloc, *status);
		if (*value == NULL)
			return (ERR_MEM);
		debug_print(DMSG_OUT, __FUNCTION__, *value);
		return (-1);
	}
	return (0);
}
