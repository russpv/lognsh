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

// Returns the args removed as a negative qty
int	lstiter_state_rwd_trim(t_state *s, t_list **lst, int (*test)(void *),
		void (*del)(t_mem_mgr *, void *))
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
