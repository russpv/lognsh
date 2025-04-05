/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   llist_utils_2.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dayeo <dayeo@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/05 21:52:00 by dayeo             #+#    #+#             */
/*   Updated: 2025/04/05 21:58:43 by dayeo            ###   ########.fr       */
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

/*LSTMAP
** Returns new (doubly) linked list head ptr using func f
** on each node of an existing list.
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
			return (err(EMSG_LMAP_MALLOC), NULL);
		}
		ft_lstadd_back(&newlst, new);
		lst = lst->next;
	}
	return (debug_print(DBGMSG_LMAP_DONE, (void *)newlst), newlst);
}

void	ft_lstprint(t_list *lst)
{
	int	count;

	count = 0;
	if (lst == NULL)
	{
		return ;
	}
	if (DEBUG)
	{
		colored_printf(RED, "linkedl: " RESET);
		while (lst)
		{
			count++;
			fprintf(stderr, YELLOW "_%s_ -> " RESET, \
					(const char *)lst->content);
			lst = lst->next;
		}
		colored_printf(RED, "printed %d nodes.\n" RESET, count);
	}
	return ;
}

void	ft_lstprinter(t_list *lst, int (*f)(void *))
{
	int	count;

	count = 0;
	if (lst == NULL)
	{
		colored_printf(RED, "nothing to print.\n" RESET);
		return ;
	}
	if (DEBUG)
	{
		colored_printf(RED, "linkedl: " RESET);
		while (lst)
		{
			count++;
			if (lst->content)
				f(lst->content);
			fprintf(stderr, YELLOW " -> " RESET);
			lst = lst->next;
		}
		colored_printf(RED, "printed %d nodes.\n" RESET, count);
	}
	return ;
}

// Prints from lst to end
void	ft_lstprint_betw(t_list *lst, t_list *end)
{
	int	count;

	count = 0;
	if (lst == NULL || end == NULL)
	{
		return ;
	}
	if (DEBUG)
	{
		colored_printf(RED, "ll segment: " RESET);
		while (lst != end->next)
		{
			count++;
			fprintf(stderr, YELLOW "%s -> " RESET, (const char *)lst->content);
			lst = lst->next;
		}
		colored_printf(RED, "printed %d nodes.\n" RESET, count);
	}
	return ;
}

/* Used in mergesort. */
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
	return (count + 1);
}
