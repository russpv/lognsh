/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   i_llist_utils_2.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dayeo <dayeo@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/05 22:23:54 by dayeo             #+#    #+#             */
/*   Updated: 2025/04/05 22:24:04 by dayeo            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "i_llist.h"

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

#define DMSG_IN "%s: Deleting node: %p (content: %p) (list: %p)\n"
#define DMSG_OUT "%s: Node deleted successfully\n"

/* LSTDELONE RWD
** Removes passed node in doubly linked list
** Assuming REAR to list iteration direction
** lst: parent list (result passed by ref here)
** node: node to free
** del: ptr to func that deletes node.content
** Handles case where same pointer passed in arg 1 and 2
*/
void	ft_lstdelone_rwd_tmp(t_mem_mgr *mgr, t_list **lst, t_list **node,
		void (*del)(t_mem_mgr *, void **))
{
	if (!mgr || !lst || !node || !(*node))
		return ;
	debugv_print(DMSG_IN, __FUNCTION__, (void *)(*node),
		(void *)((*node)->content), (void *)lst);
	if ((*node)->prev)
		(*node)->prev->next = (*node)->next;
	if ((*node)->next)
		(*node)->next->prev = (*node)->prev;
	if ((*node)->content)
		del(mgr, &(*node)->content);
	if (*lst == *node)
		*lst = (*node)->next;
	mgr->dealloc(&mgr->list, *node);
	*node = NULL;
	debug_print(DMSG_OUT, __FUNCTION__);
}

/*LSTMAP
** Returns new (doubly) linked list list ptr using func f
** on each node of an existing list.
** lst: ptr to node
** f: function applied to each node.content
** UNPROTECTED: returns NULL only when !lst
*/
t_list	*ft_lstmap_tmp(t_mem_mgr *mgr, t_list *lst, void *(*f)(t_mem_mgr *,
			const void *), void (*del)(t_mem_mgr *, void **))
{
	t_list	*newlst;
	t_list	*new;
	void	*new_content;

	newlst = NULL;
	if (!lst || !mgr)
		return (NULL);
	while (lst)
	{
		new_content = f(mgr, lst->content);
		new = ft_lstnew_tmp(mgr, new_content);
		if (!new)
		{
			(mgr->dealloc)(&mgr->list, new_content);
			if (newlst)
			{
				ft_lstclear_tmp(mgr, &newlst, del);
				newlst = NULL;
			}
			return (err("Malloc"), NULL);
		}
		ft_lstadd_back(&newlst, new);
		lst = lst->next;
	}
	return (newlst);
}

#define DBGMSG_LSTINSRWD_ANNOUNCE "%s: got lst: %p, list:%p.\n"
#define DBGMSG_LSTINSRWD_NULLARG "%s: NULL input.\n"
#define DBGMSG_LSTINSRWD_ENDNODE "%s: Starting from end: %p\n"
#define DBGMSG_LSTINSRWD_PREF \
	"%s: Applying f to node: %p, content: \
	%p\n"
#define DBGMSG_LSTINSRWD_POSTF \
	"%s: f applied to node: \
	%p\n"
#define DBGMSG_LSTINSRWD_SUCCESS "%s: Inserts done successfully.\n"

/* Iterates BACKWARDS and applies insertion func f */
void	ft_lstiter_ins_rwd_tmp(t_mem_mgr *mgr, t_list **lst,
		int (*f)(t_mem_mgr *, t_list **, void *))
{
	t_list	*lst_rear;
	t_list	*tmp;

	if (lst == NULL)
		return (debug_print(DBGMSG_LSTINSRWD_NULLARG, __FUNCTION__));
	lst_rear = ft_lstlast(*lst);
	debugv_print(DBGMSG_LSTINSRWD_ENDNODE, __FUNCTION__, (void *)lst_rear);
	while (lst_rear)
	{
		tmp = lst_rear->prev;
		debugv_print(DBGMSG_LSTINSRWD_PREF, __FUNCTION__, (void *)lst_rear,
			(void *)lst_rear->content);
		f(mgr, &(lst_rear), lst_rear->content);
		debugv_print(DBGMSG_LSTINSRWD_POSTF, __FUNCTION__, (void *)lst_rear);
		if (NULL == tmp)
			*lst = lst_rear;
		lst_rear = tmp;
	}
	debug_print(DBGMSG_LSTINSRWD_SUCCESS, __FUNCTION__);
}
