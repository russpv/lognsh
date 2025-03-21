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

// TODO FIX THE DELETE AND INVALID LIST PREV ISSUE
int	lstiter_state_rwd_trim(t_state *s, t_list **lst, int (*test)(void *),
		void (*del)(t_mem_mgr *, void *))
{
	int			res;
	t_mem_mgr	*m;
	t_list		*lst_rear;
	t_list		*tmp;

	m = get_mem(s);
	if (lst == NULL)
		return (ERR_ARGS);
	lst_rear = ft_lstlast(*lst);
	while (lst_rear)
	{
		res = 0;
		fprintf(stderr, "HAHA\n");
		tmp = lst_rear->prev;
		res = test((lst_rear)->content);
		if (res > 0)
		{
			fprintf(stderr, "HOHO\n");
			// ft_lstdelone_rwd_tmp(m, lst, lst_rear, del);
		}
		if (res < 0)
			return (ERR_GENERAL);
		if (NULL == tmp)
			*lst = lst_rear;
		lst_rear = tmp;
	}
	return (0);
}
