#include "libft.h"

/* LSTITER
** Iterates singly linked list and applies func f to node.content
** lst: ptr to node
** f: ptr to function
** UNPROTECTED
*/
void	ft_lstiter(t_list *lst, void (*f)(void *))
{
	if (lst == NULL)
		return ;
	while (lst)
	{
		f(lst->content);
		lst = lst->next;
	}
}
