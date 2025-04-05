/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   llist_utils_1.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dayeo <dayeo@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/05 09:32:15 by dayeo             #+#    #+#             */
/*   Updated: 2025/04/05 21:53:02 by dayeo            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "llist_int.h"

#define DBGMSG_LSIZE_NULL "ft_lstsize: List is NULL, returning 0\n"
#define DBGMSG_LSIZE_START "Starting ft_lstsize traversal. Initial lst: %p\n"
#define DBGMSG_LSIZE_ONNODE "Visiting node %p (next: %p)\n"
#define DBGMSG_LSIZE_DONE "Finished traversal. Node count: %d\n"
#define DBGMSG_LMAP_NULLARG "ft_lstmap: NULL input.\n"
#define EMSG_LMAP_MALLOC "ft_lstmap: Malloc err.\n"
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

/* LSTFIRST
** Returns last node of a list
*/
t_list	*ft_lstfirst(t_list *lst)
{
	if (!lst)
		return (NULL);
	while (lst->prev)
		lst = lst->prev;
	return (lst);
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

/* Returns new linked list node with f(content). */
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
		debugv_print(DBGMSG_LSIZE_NULL);
		return (0);
	}
	debugv_print(DBGMSG_LSIZE_START, (void *)lst);
	while (lst)
	{
		debugv_print(DBGMSG_LSIZE_ONNODE, (void *)lst, (void *)lst->next);
		count++;
		lst = lst->next;
	}
	debugv_print(DBGMSG_LSIZE_DONE, count);
	return (count);
}
