/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   i_llist.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rpeavey <rpeavey@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/10 16:30:04 by rpeavey           #+#    #+#             */
/*   Updated: 2025/04/10 16:30:05 by rpeavey          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "i_llist.h"

/* LSTNEW
** Returns new linked list list ptr with one node
** But content is not copied
*/
t_list	*ft_lstnew_tmp(t_mem_mgr *mgr, void *content)
{
	t_list	*ll;

	ll = mgr->f(&mgr->list, sizeof(t_list));
	if (!ll)
		return (NULL);
	ll->next = NULL;
	ll->prev = NULL;
	ll->content = content;
	return (ll);
}

/* LSTDELONE
** Removes passed node in doubly linked list
** Assuming front to rear iteration direction
** lst: parent list
** node: node to free
** del: ptr to func that deletes node.content
*/
void	ft_lstdelone_tmp(t_mem_mgr *m, t_list **lst, t_list *node,
		void (*del)(t_mem_mgr *, void **))
{
	if (!m || !lst || !node)
		return ;
	if (!*lst)
		return ;
	if (node->prev)
		node->prev->next = node->next;
	if (node->next)
		node->next->prev = node->prev;
	if (node == *lst)
		*lst = node->next;
	del(m, &node->content);
	m->dealloc(&m->list, node);
}

/* LSTCLEAR
** Deletes all subsequent nodes and current node (entire list if head)
** lst: ptr to ptr to node
** del: ptr to free(content) function
*/
void	ft_lstclear_tmp(t_mem_mgr *mgr, t_list **lst, void (*del)(t_mem_mgr *m,
			void **))
{
	t_list	*tmp;
	t_list	*next;

	if (!mgr || !*lst || !lst)
		return ;
	dvprint(DMSG_GOT, __FUNCTION__, (void *)lst, (void *)*lst);
	tmp = *lst;
	while (tmp)
	{
		dvprint(DMSG_DEL, __FUNCTION__, (void *)tmp);
		next = tmp->next;
		ft_lstdelone_tmp(mgr, lst, tmp, del);
		tmp = next;
	}
	dprint("%s: Lst deleted successfully\n", __FUNCTION__);
	*lst = NULL;
}

// Helper function: Deep copy a single node's content
t_list	*ft_lstcopy_node_tmp(t_mem_mgr *mgr, const t_list *orig,
		void *(*f)(t_mem_mgr *, const void *))
{
	t_list	*new_node;

	new_node = mgr->f(&mgr->list, sizeof(t_list));
	if (!new_node)
		return (NULL);
	new_node->content = f(mgr, orig->content);
	if (!new_node->content)
	{
		mgr->dealloc(&mgr->list, new_node);
		return (NULL);
	}
	new_node->next = NULL;
	new_node->prev = NULL;
	return (new_node);
}

// Uses copy function f
t_list	*ft_lstcopy_tmp(t_mem_mgr *mgr, t_list *orig, void *(*f)(t_mem_mgr *,
			const void *), void (*del)(t_mem_mgr *, void **))
{
	t_list	*new_list;
	t_list	*prev_node;
	t_list	*new_node;

	new_list = NULL;
	prev_node = NULL;
	while (orig)
	{
		new_node = ft_lstcopy_node_tmp(mgr, orig, f);
		if (!new_node)
		{
			ft_lstclear_tmp(mgr, &new_list, del);
			return (NULL);
		}
		if (prev_node)
		{
			prev_node->next = new_node;
			new_node->prev = prev_node;
		}
		else
			new_list = new_node;
		prev_node = new_node;
		orig = orig->next;
	}
	return (new_list);
}
