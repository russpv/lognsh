#include "llist.h"

#define DBGMSG_LSIZE_NULL "ft_lstsize: List is NULL, returning 0\n"
#define DBGMSG_LSIZE_START "Starting ft_lstsize traversal. Initial lst: %p\n"
#define DBGMSG_LSIZE_ONNODE "Visiting node %p (next: %p)\n"
#define DBGMSG_LSIZE_DONE "Finished traversal. Node count: %d\n"
#define DBGMSG_LMAP_NULLARG "ft_lstmap: NULL input.\n"
#define ERRMSG_LMAP_MALLOC "ft_lstmap: Malloc err.\n"
#define DBGMSG_LMAP_DONE "ft_lstmap: Modded: %p\n"

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
		return (debug_print(DBGMSG_LMAP_NULLARG), NULL);
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
			return (err(ERRMSG_LMAP_MALLOC), NULL);
		}
		ft_lstadd_back(&newlst, new);
		lst = lst->next;
	}
	return (debug_print(DBGMSG_LMAP_DONE, (void *)newlst), newlst);
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
		debug_print(DBGMSG_LSIZE_NULL);
		return (0);
	}
	debug_print(DBGMSG_LSIZE_START, (void *)lst);
	while (lst)
	{
		debug_print(DBGMSG_LSIZE_ONNODE, (void *)lst,
			(void *)lst->next);
		count++;
		lst = lst->next;
	}
	debug_print(DBGMSG_LSIZE_DONE, count);
	return (count);
}


void	ft_lstprint(t_list *lst)
{
	int count;

	count = 0;
	if (lst == NULL)
	{
		return ;
	}
	if (DEBUG)
	{
		fprintf(stderr,RED"linkedl: "RESET);
		while (lst)
		{
			count++;
			fprintf(stderr, YELLOW"%s -> "RESET, lst->content);
			lst = lst->next;
		}
		fprintf(stderr, RED"printed %d nodes.\n"RESET, count);
	}
	return ;
}

// Prints from lst to end
void	ft_lstprint_betw(t_list *lst, t_list *end)
{
	int count;

	count = 0;
	if (lst == NULL || end == NULL)
	{
		return ;
	}
	if (DEBUG)
	{
		fprintf(stderr,RED"ll segment: "RESET);
		while (lst != end->next)
		{
			count++;
			fprintf(stderr, YELLOW"%s -> "RESET, lst->content);
			lst = lst->next;
		}
		fprintf(stderr, RED"printed %d nodes.\n"RESET, count);
	}
	return ;
}

int	ft_lstsize_betw(t_list *lst, t_list *last)
{
	int	count;

	count = 0;
	if (lst == NULL)
	{
		return (0);
	}
	while (lst && lst != last)
	{
		count++;
		lst = lst->next;
	}
	return (count);
}
