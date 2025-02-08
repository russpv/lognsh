#include "llist.h"

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

/* Adds llist in lst */
void	ft_lstadd_insert(t_list **lst, t_list *new)
{
	t_list *save_next = NULL;
	t_list *last_new_node = NULL;
	
	debug_print("[DEBUG] ft_lstadd_insert got lst = %p, head = %p\n", (void*)lst, (void*)*lst);
	// Check if the list or new node is NULL
	if (!lst || !new)
	{
		debug_print("[DEBUG] NULL input detected: lst = %p, new = %p\n", (void*)lst, (void*)new);
		return ;
	}

	// If the list is empty, insert the new node as the first element
	if (*lst == NULL)
	{
		*lst = new;
		new->prev = NULL;
		debug_print("[DEBUG] List is empty. Added new node as the first element: %p\n", (void*)new);
		debug_print("[DEBUG] Insert completed successfully.\n");
		return ;
	}

	// Find the last node of the new list
	last_new_node = ft_lstlast(new);
	debug_print("[DEBUG] Last node in new list: %p\n", (void*)last_new_node);

	// Check if the current list has a next node
	if ((*lst)->next)
	{
		save_next = (*lst)->next;
		debug_print("[DEBUG] Current list has a next node. Save next node: %p\n", (void*)save_next);
	}

	// Insert the new node after the first node
	(*lst)->next = new;
	new->prev = *lst;
	debug_print("[DEBUG] Inserted new node after the first node. new->prev = %p, (*lst)->next = %p\n", (void*)new->prev, (void*)(*lst)->next);

	// Link the last node of the new node to the saved next node, if there was one
	last_new_node->next = save_next;
	if (save_next)
	{
		save_next->prev = last_new_node;
		debug_print("[DEBUG] Linked last node of new list to the saved next node: save_next->prev = %p\n", (void*)save_next->prev);
	}

	debug_print("[DEBUG] Insert completed successfully.\n");
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
	debug_print("[DEBUG] ft_lstclear got lst = %p, head = %p\n", (void*)lst, (void*)*lst);

	tmp = *lst;
	while (tmp)
	{
		debug_print("[DEBUG] ft_lstclear deleting node = %p\n", (void*)tmp);
		next = tmp->next;
		ft_lstdelone(lst, tmp, del);
		tmp = next;
	}
	*lst = NULL;
}

/* LSTDELONE
** Removes passed node in doubly linked list
** Assuming head to rear iteration direction
** lst: parent list
** node: node to free
** del: ptr to func that deletes node.content
*/

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
	node = NULL;
}
/* LSTDELONE
** Removes passed node in doubly linked list
** Assuming REAR to HEAD iteration direction
** lst: parent list
** node: node to free
** del: ptr to func that deletes node.content
** Handles case where same pointer passed in arg 1 and 2
*/

void	ft_lstdelone_rwd(t_list **lst, t_list **node, void (*del)(void *))
{
	t_list *tmp;
	debug_print("[DEBUG] ft_lstdelone_rwd got (head: %p)\n", (void*)lst);

	if (!lst || !node || !(*node) || !del)
		return ;
	debug_print("[DEBUG] Deleting node: %p (content: %p) (head: %p)\n", (void*)(*node), (void*)((*node)->content), (void*)lst);
	tmp = (*node)->prev;
    if ((*node)->prev) {
		debug_print("[DEBUG] Setting prev->next (%p -> %p)\n", (void*)(*node)->prev, (void*)(*node)->next);
        (*node)->prev->next = (*node)->next;
	}
    if ((*node)->next) {
		debug_print("[DEBUG] Setting next->prev (%p -> %p)\n", 
			(void*)(*node)->next, (void*)(*node)->prev);
        (*node)->next->prev = (*node)->prev;
	}
    if ((*node)->content) {
		debug_print("[DEBUG] Freeing content at %p\n", (void*)(*node)->content);
		del((*node)->content);
	}
	debug_print("[DEBUG] Freeing node at %p\n", (void*)(*node));
    free(*node);
	*node = NULL;
	if (*lst == *node) {
		debug_print("[DEBUG] Adjusting head: old head=%p, new head=%p\n", 
		(void*)(*lst), (void*)(tmp));		
		*lst = tmp;
	}
	debug_print("[DEBUG] Node deleted successfully\n");

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
		f((lst)->content);
		lst = (lst)->next;
	}
}

/* Iterates BACKWARDS and applies f */
void	ft_lstiter_ins_rwd(t_list **lst, void (*f)(t_list **, void *))
{
	t_list *lst_rear;
	t_list *tmp;

	debug_print("[DEBUG] ft_lstiter_ins_rwd got lst: %p, head:%p.\n", (void*)lst, (void*)*lst);

	// Check if the input list is NULL
	if (lst == NULL)
	{
		debug_print("[DEBUG] Input list is NULL. Returning.\n");
		return ;
	}

	// Initialize lst_rear to the last node
	lst_rear = ft_lstlast(*lst);
	debug_print("[DEBUG] Starting iteration from last node: %p\n", (void*)lst_rear);

	// Iterate backwards through the list
	while (lst_rear)
	{
		// Save the previous node before applying the function
		tmp = lst_rear->prev;

		// Print the current node's address and content
		debug_print("[DEBUG] Iterating node: %p, content: %p\n", (void*)lst_rear, (void*)lst_rear->content);

		// Apply the function f to the node
		f(&(lst_rear), lst_rear->content);
		debug_print("[DEBUG] Function f applied to node: %p\n", (void*)lst_rear);

		// Move to the previous node in the list
		if (NULL == tmp)
			*lst = lst_rear;
		lst_rear = tmp;
	}
	// Finished iterating through the list
	debug_print("[DEBUG] ft_lstiter_ins_rwd Iteration complete.\n");
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
