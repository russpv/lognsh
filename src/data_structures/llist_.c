/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   llist_.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rpeavey <rpeavey@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/10 16:30:14 by rpeavey           #+#    #+#             */
/*   Updated: 2025/04/10 16:30:15 by rpeavey          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "llist_int.h"

#define MSG_EMPT "%s: List is empty. Added new node as the first el:%p\n"
#define MSG_INS "%s: ins new node after 1st, new->prev=%p, *lst->next=%p\n"

/* LSTADD_BACK
** Returns single node for empty list, or adds new to end of
** linked list, keeps ptr to head ptr intact
*/
int	ft_lstadd_back(t_list **lst, t_list *new)
{
	t_list	*tmp;

	if (new == NULL)
		return (-1);
	if (lst == NULL)
		return (-1);
	if (*lst == NULL)
	{
		*lst = new;
		return (1);
	}
	tmp = ft_lstlast(*lst);
	tmp->next = new;
	new->prev = tmp;
	return (0);
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
	dvprint("%s: got lst:%p (head:%p)\n", (void *)lst, (void *)*lst);
	if (!lst || !new)
		return (dvprint("%s: null input lst:%p, new:%p\n", __FUNCTION__,
				(void *)lst, (void *)new));
	if (*lst == NULL)
	{
		*lst = new;
		new->prev = NULL;
		return (dvprint(MSG_EMPT, __FUNCTION__, (void *)new));
	}
	last_new_node = ft_lstlast(new);
	if ((*lst)->next)
		save_next = (*lst)->next;
	(*lst)->next = new;
	new->prev = *lst;
	dvprint(MSG_INS, __FUNCTION__, (void *)new->prev, (void *)(*lst)->next);
	last_new_node->next = save_next;
	if (save_next)
		save_next->prev = last_new_node;
	dprint("%s: insert done.\n", __FUNCTION__);
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
		dvprint("%s: deleting node:%p\n", __FUNCTION__, (void *)tmp);
		next = tmp->next;
		ft_lstdelone(lst, tmp, del);
		tmp = next;
	}
	*lst = NULL;
}
