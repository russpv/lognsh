#include "libft.h"

/*LSTMAP
** Returns modified singly linked list head ptr using func f
** lst: ptr to node
** f: function applied to each node.content
** del: used if allocation fails
** UNPROTECTED: returns NULL only when !lst
*/

t_list	*ft_lstmap(t_list *lst, void *(*f)(void *), void (*del)(void *))
{
	t_list	*newlst;
	t_list	*new;
	void	*new_content;

	if (!lst)
		return (NULL);
	newlst = NULL;
	while (lst)
	{
		new_content = f(lst->content);
		new = ft_lstnew(new_content);
		if (!new)
		{
			(*del)(new_content);
			if (newlst)
			{
				ft_lstclear(&newlst, del);
				newlst = NULL;
			}
			return (NULL);
		}
		ft_lstadd_back(&newlst, new);
		lst = lst->next;
	}
	return (newlst);
}
