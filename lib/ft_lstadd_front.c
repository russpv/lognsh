#include "libft.h"

/* LSTADD_FRONT
** Adds new node at the beginning of linked list
** lst: location of first node
** new: location of node to be added
** PROTECTED: if lst is NULL, pushes onto empty list
** if new is NULL, does nothing
*/

void	ft_lstadd_front(t_list **lst, t_list *new)
{
	if (!lst || !new)
		return ;
	new->next = *lst;
	*lst = new;
}
