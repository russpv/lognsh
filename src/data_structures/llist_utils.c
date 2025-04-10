/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   llist_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rpeavey <rpeavey@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/10 16:31:16 by rpeavey           #+#    #+#             */
/*   Updated: 2025/04/10 16:31:17 by rpeavey          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "llist_int.h"

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

/* LSTSIZE
** Returns the node count of the linked list
*/
int	ft_lstsize(t_list *lst)
{
	int	count;

	count = 0;
	if (lst == NULL)
		return (dvprint("%s: NULL lst.\n", __FUNCTION__), 0);
	dvprint("%s: traversing lst: %p\n", __FUNCTION__, (void *)lst);
	while (lst)
	{
		dvprint("%s: visiting node:%p (-> %p)\n", __FUNCTION__, (void *)lst,
			(void *)lst->next);
		count++;
		lst = lst->next;
	}
	dvprint("%s: count:%d nodes.\n", __FUNCTION__, count);
	return (count);
}

/* Used in mergesort. */
int	ft_lstsize_betw(t_list *lst, t_list *last)
{
	int	count;

	count = 0;
	if (lst == NULL)
		return (0);
	while (lst && lst != last)
	{
		count++;
		lst = lst->next;
	}
	return (count + 1);
}
