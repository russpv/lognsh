#include "../include/libft.h"


/* LSTDELONE
** Removes passed node
** 1st: node to free
** del: ptr to func that deletes node.content
** UNPROTECTED Does not check del
*/

void	ft_lstdelone(t_list *lst, void (*del)(void *))
{
	if (lst == NULL)
		return ;
	del(lst->content);
	free(lst);
	lst = NULL;
}
