#include "llist_int.h"

#define DEBUGMSG_INSERT_GOT "ft_lstadd_insert got lst = %p, head = %p\n"
#define DEBUGMSG_INSERT_NULL "NULL input detected: lst = %p, new = %p\n"
#define DEBUGMSG_INSERT_EMPTY "List is empty. Added new node as the first element:%p\n" 
#define DEBUGMSG_INSERTED "Inserted new node after the first node. new->prev =%p, (*lst)->next = %p\n"
#define DEBUGMSG_INSERT_0 "ft_lstadd_insert completed successfully.\n"
#define DEBUGMSG_CLR_DEL "ft_lstclear deleting node = %p\n"
#define DEBUGMSG_CLR_GOT "ft_lstclear got lst = %p, head = %p\n"

/* LSTADD_BACK
** Returns single node for empty list, or adds new to end of
** linked list, keeps ptr to head ptr intact
** TODO: error propagation
*/
void	ft_lstadd_back(t_list **lst, t_list *new)
{
	t_list	*tmp;

	if (new == NULL)
		return ;
	if (lst == NULL)
		write(2, "WTF", 3);
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

/* Adds llist within lst */
void	ft_lstadd_insert(t_list **lst, t_list *new)
{
	t_list	*save_next;
	t_list	*last_new_node;

	save_next = NULL;
	last_new_node = NULL;
	debug_print(DEBUGMSG_INSERT_GOT, (void *)lst, (void *)*lst);
	if (!lst || !new)
		return (debug_print(DEBUGMSG_INSERT_NULL, (void *)lst, (void *)new));
	if (*lst == NULL)
	{
		*lst = new;
		new->prev = NULL;
		return (debug_print(DEBUGMSG_INSERT_EMPTY, (void *)new));
	}
	last_new_node = ft_lstlast(new);
	if ((*lst)->next)
		save_next = (*lst)->next;
	(*lst)->next = new;
	new->prev = *lst;
	debug_print(DEBUGMSG_INSERTED, (void *)new->prev, (void *)(*lst)->next);
	last_new_node->next = save_next;
	if (save_next)
		save_next->prev = last_new_node;
	debug_print(DEBUGMSG_INSERT_0);
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
	debug_print(DEBUGMSG_CLR_GOT, (void *)lst,
		(void *)*lst);
	tmp = *lst;
	while (tmp)
	{
		debug_print(DEBUGMSG_CLR_DEL, (void *)tmp);
		next = tmp->next;
		ft_lstdelone(lst, tmp, del);
		tmp = next;
	}
	*lst = NULL;
}
