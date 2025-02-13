#include "llist.h"

/* LSTLAST
** Returns last node of a list
*/
t_list	*ft_lstlast(t_list *lst)
{
	if (!lst)
		return (NULL);
	while (lst->next)
		lst = lst->next;
	return (lst);
}

/*LSTMAP
** Returns modified (doubly) linked list head ptr using func f
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

	newlst = NULL;
	if (!lst)
		return (debug_print("ft_lstmap: NULL input.\n"), NULL);
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
			return (debug_print("ft_lstmap: Malloc err.\n"), NULL);
		}
		ft_lstadd_back(&newlst, new);
		lst = lst->next;
	}
	return (debug_print("ft_lstmap: Modded: %p\n", (void *)newlst), newlst);
}

/* LSTNEW
** Returns new linked list head ptr with one node
** But content is not copied
*/
t_list	*ft_lstnew(void *content)
{
	t_list	*ll;

	ll = malloc(sizeof(t_list));
	if (!ll)
		return (NULL);
	ll->next = NULL;
	ll->prev = NULL;
	ll->content = content;
	return (ll);
}

/* Returns new linked list node with new content. */
t_list	*ft_lstnew_copystr(void *content, char *(*f)(const char *))
{
	t_list	*new_node;

	new_node = malloc(sizeof(t_list));
	if (!new_node)
		return (NULL);
	new_node->content = f(content);
	if (!new_node->content)
	{
		free(new_node);
		return (NULL);
	}
	new_node->next = NULL;
	new_node->prev = NULL;
	return (new_node);
}

/* LSTSIZE
** Returns the node count of the linked list
*/
int	ft_lstsize(t_list *lst)
{
	int	count;

	count = 0;
	if (lst == NULL)
	{
		debug_print("[DEBUG] ft_lstsize: List is NULL, returning 0\n");
		return (0);
	}
	debug_print("[DEBUG] Starting ft_lstsize traversal. Initial lst: %p\n",
		(void *)lst);
	while (lst)
	{
		debug_print("[DEBUG] Visiting node %p (next: %p)\n", (void *)lst,
			(void *)lst->next);
		count++;
		lst = lst->next;
	}
	debug_print("[DEBUG] Finished traversal. Node count: %d\n", count);
	return (count);
}
