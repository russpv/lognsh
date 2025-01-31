#include "../include/libft.h"


/* LSTADD_BACK
** Returns single node for empty list, or adds new to back of
** linked list, keeps ptr to head ptr intact
*/

void	ft_lstadd_back(t_list **lst, t_list *new)
{
	t_list	*tmp;

	if (new == NULL)
		return ;
	if (*lst == NULL)
	{
		*lst = new;
		return ;
	}
	tmp = ft_lstlast(*lst);
	tmp->next = new;
}
