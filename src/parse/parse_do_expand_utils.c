#include "parse_int.h"

/* Custom llist iterator with t_state function ptr */
int	lstiter_state(t_state *s, t_list *lst, \
							int (*f)(t_state *, void *))
{
	int res;

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
	return (0);
}
