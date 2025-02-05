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
	debug_print("[DEBUG] ft_lstmap.\n");
	// Check if the input list is NULL
	if (!lst)
	{
		debug_print("[DEBUG] Input list is NULL. Returning NULL.\n");
		return (NULL);
	}

	// Initialize the new list
	newlst = NULL;

	// Loop through the original list
	while (lst)
	{
		debug_print("[DEBUG] Mapping node: %p, content: %p\n", (void*)lst, (void*)lst->content);

		// Apply the function to the content
		new_content = f(lst->content);
		if (!new_content)
		{
			debug_print("[DEBUG] Function f returned NULL for content: %p\n", (void*)lst->content);
		}

		// Create a new node with the mapped content
		new = ft_lstnew(new_content);
		if (!new)
		{
			debug_print("[DEBUG] Memory allocation failed for new node. Cleaning up.\n");
			// If new node creation fails, delete the content and clear the list
			(*del)(new_content);
			if (newlst)
			{
				ft_lstclear(&newlst, del);
				newlst = NULL;
			}
			return (NULL);
		}

		// Add the new node to the new list
		ft_lstadd_back(&newlst, new);
		debug_print("[DEBUG] Added new node to new list: %p\n", (void*)new);

		// Move to the next node in the original list
		lst = lst->next;
	}

	// Return the new list
	debug_print("[DEBUG] Mapping complete. Returning new list: %p\n", (void*)newlst);
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
	ll->prev = NULL;
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
	if (lst == NULL) {
		debug_print("[DEBUG] List is NULL, returning 0\n");
		return (0);
	}
	debug_print("[DEBUG] Starting ft_lstsize traversal. Initial lst: %p\n", (void*)lst);
	while (lst)
	{
		debug_print("[DEBUG] Visiting node %p (next: %p)\n", (void*)lst, (void*)lst->next);
		count++;
		lst = lst->next;
	}
	debug_print("[DEBUG] Finished traversal. Node count: %d\n", count);
	return (count);
}
