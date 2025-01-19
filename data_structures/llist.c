#include "llist.h"

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

/* LSTCLEAR
** Deletes all subsequent nodes and current node
** lst: ptr to ptr to node
** del: ptr to free(content) function
*/

void	ft_lstclear(t_list **lst, void (*del)(void *))
{
	t_list	*tmp;
	t_list	*prev;

	if (!*lst || !del || !lst)
		return ;
	tmp = *lst;
	while (tmp)
	{
		prev = tmp;
		tmp = tmp->next;
		ft_lstdelone(prev, del);
	}
	*lst = NULL;
}

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

/* LSTNEW
** Returns new linked list head ptr with one node
*/

t_list	*ft_lstnew(void *content)
{
	t_list	*ll;

	ll = malloc(sizeof(t_list));
	if (!ll)
		return (NULL);
	ll->next = NULL;
	ll->content = content;
	return (ll);
}

/* LSTSIZE
** Returns the node count of the linked list
*/

int	ft_lstsize(t_list *lst)
{
	int	count;

	count = 0;
	if (lst == NULL)
		return (0);
	while (lst)
	{
		count++;
		lst = lst->next;
	}
	return (count);
}
