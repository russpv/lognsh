#include "llist.h"

/* LSTADD_BACK
** Returns single node for empty list, or adds new to end of
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
	new->prev = tmp;
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
	if (*lst == NULL)
	{
		*lst = new;
		new->prev = NULL;
		return ;
	}
	new->next = *lst;
	(*lst)->prev = new;
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
	t_list	*next;

	if (!*lst || !del || !lst)
		return ;
	tmp = *lst;
	while (tmp)
	{
		next = tmp->next;
		ft_lstdelone(lst, tmp, del);
		tmp = next;
	}
	*lst = NULL;
}

/* LSTDELONE
** Removes passed node
** 1st: node to free
** del: ptr to func that deletes node.content
*/
#include <assert.h>

void	ft_lstdelone(t_list **lst, t_list *node, void (*del)(void *))
{
	if (!lst || !node || !del)
		return ;
    if (node->prev)
        node->prev->next = node->next;
    if (node->next)
        node->next->prev = node->prev;
    if (node == *lst)
        *lst = node->next;
    del(node->content);
    free(node);
}

/* LSTITER
** Iterates linked list and applies func f to node.content
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

/* LSTITERSTR
** Iterates linked list and applies func f to node.content
** Returns the node content if the func f returns error code
** lst: ptr to node
** f: ptr to function
** UNPROTECTED
*/
char	*ft_lstiterstr(t_list *lst, int (*f)(void *))
{
	if (lst == NULL)
		return (NULL);
	while (lst)
	{
		if (0 != f(lst->content))
			return (lst->content);
		lst = lst->next;
	}
	return (NULL);
}
