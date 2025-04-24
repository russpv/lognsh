
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
int	ft_lstiter_state_ins_rwd_mem(t_state *s, t_list **lst, int (*f)(t_state *,
			t_list **, void *))
{
	t_list	*lst_rear;
	t_list	*tmp;
	int		res;

	if (lst == NULL)
		return (dprint("Null arg\n", __FUNCTION__), ERR_ARGS);
	lst_rear = ft_lstlast(*lst);
	dvprint(_MOD_ ": %s: %p\n", __FUNCTION__, (void *)lst_rear);
	while (lst_rear)
	{
		tmp = lst_rear->prev;
		dvprint(_MOD_ "%s: %p, content:%p\n", __FUNCTION__, (void *)lst_rear,
			(void *)lst_rear->content);
		res = f(s, &(lst_rear), lst_rear->content);
		dvprint(_MOD_ "%s: Applied f to %p\n", __FUNCTION__, (void *)lst_rear);
		if (0 != res)
			return (res);
		if (NULL == tmp)
			*lst = lst_rear;
		lst_rear = tmp;
	}
	dprint(_MOD_ ": %s: Inserts done successfully.\n", __FUNCTION__);
	return (0);
}

// Returns the args removed as a negative qty
int	lstiter_state_rwd_trim(t_state *s, t_list **lst, int (*test)(void *),
		void (*del)(t_mem_mgr *, void **))
{
	int		res;
	t_list	*lst_rear;
	t_list	*tmp;
	int		args_removed;

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

// Called on lst->content for inserting combined subtokens
// Requires tok to be a NORMAL tok
void	*token_to_redir(t_mem_mgr *m, const void *tok)
{
	if (!tok)
		return (NULL);
	return (create_redir_data_node_deep(m, tok_get_raw((t_tok *)tok)));
}

// Called on lst->content; requires tok to be a NORMAL tok
void	*token_to_arg(t_mem_mgr *m, const void *tok)
{
	if (!tok)
		return (NULL);
	return (create_arg_data_node_deep(m, tok_get_raw((t_tok *)tok)));
}
