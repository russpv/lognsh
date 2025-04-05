/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   i_llist.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dayeo <dayeo@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/05 09:28:29 by dayeo             #+#    #+#             */
/*   Updated: 2025/04/05 11:37:08 by dayeo            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "i_llist.h"

#define DMSG_GOT "%s: got lst = %p, list = %p\n"
#define DMSG_DEL "%s: deleting node = %p\n"

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
	debugv_print(DMSG_GOT, __FUNCTION__, (void *)lst, (void *)*lst);
	tmp = *lst;
	while (tmp)
	{
		debugv_print(DMSG_DEL, __FUNCTION__, (void *)tmp);
		next = tmp->next;
		ft_lstdelone_tmp(mgr, lst, tmp, del);
		tmp = next;
	}
	debug_print("%s: Lst deleted successfully\n", __FUNCTION__);
	*lst = NULL;
}

/* LSTDELONE
** Removes passed node in doubly linked list
** Assuming list to rear iteration direction
** lst: parent list
** node: node to free
** del: ptr to func that deletes node.content
*/
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

/* Returns new linked list node with f(content). */
t_list	*ft_lstnew_copystr_mem(t_mem_mgr *m, void *content,
		char *(*f)(t_mem_mgr *, const char *))
{
	t_list	*new_node;

	new_node = m->f(&m->list, sizeof(t_list));
	if (!new_node)
		return (NULL);
	new_node->content = f(m, content);
	if (!new_node->content)
	{
		m->dealloc(&m->list, new_node);
		return (NULL);
	}
	new_node->next = NULL;
	new_node->prev = NULL;
	return (new_node);
}
