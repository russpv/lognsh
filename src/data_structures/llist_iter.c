/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   llist_iter.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rpeavey <rpeavey@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/10 16:30:25 by rpeavey           #+#    #+#             */
/*   Updated: 2025/04/10 16:30:26 by rpeavey          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "llist_int.h"

/* LSTITER
** Iterates linked list and applies func f to node.content
** lst: ptr to node
** f: ptr to function
** UNPROTECTED
*/
int	ft_lstiter(t_list *lst, int (*f)(void *))
{
	if (lst == NULL)
		return (-1);
	while (lst)
	{
		if (0 != f((lst)->content))
			return (-1);
		lst = (lst)->next;
	}
	return (0);
}

/* Iterates BACKWARDS and applies f */
void	ft_lstiter_ins_rwd(t_list **lst, int (*f)(t_list **, void *))
{
	t_list	*lst_rear;
	t_list	*tmp;

	dprint("%s: got lst:%p, head:%p.\n", __FUNCTION__, (void *)lst,
		(void *)*lst);
	if (lst == NULL)
		return (dprint("%s: NULL list.\n", __FUNCTION__));
	lst_rear = ft_lstlast(*lst);
	dprint("%s: starting from end: %p\n", __FUNCTION__, (void *)lst_rear);
	while (lst_rear)
	{
		tmp = lst_rear->prev;
		dprint("%s: doing f(node: %p, content:%p)\n", __FUNCTION__,
			(void *)lst_rear, (void *)lst_rear->content);
		f(&(lst_rear), lst_rear->content);
		if (NULL == tmp)
			*lst = lst_rear;
		lst_rear = tmp;
	}
	dprint("%s: %s: done.\n", __FUNCTION__);
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
