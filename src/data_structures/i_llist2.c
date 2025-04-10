/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   i_llist2.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rpeavey <rpeavey@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/10 16:30:11 by rpeavey           #+#    #+#             */
/*   Updated: 2025/04/10 16:30:12 by rpeavey          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "i_llist.h"

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
	dvprint(DMSG_IN, __FUNCTION__, (void *)(*node), (void *)((*node)->content),
		(void *)lst);
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
	dprint(DMSG_OUT, __FUNCTION__);
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

/* Iterates BACKWARDS and applies insertion func f */
void	ft_lstiter_ins_rwd_tmp(t_mem_mgr *mgr, t_list **lst,
		int (*f)(t_mem_mgr *, t_list **, void *))
{
	t_list	*lst_rear;
	t_list	*tmp;

	if (lst == NULL)
		return (dprint("%s: NULL list.\n", __FUNCTION__));
	lst_rear = ft_lstlast(*lst);
	dvprint("%s: inserting from end: %p\n", __FUNCTION__, (void *)lst_rear);
	while (lst_rear)
	{
		tmp = lst_rear->prev;
		f(mgr, &(lst_rear), lst_rear->content);
		if (NULL == tmp)
			*lst = lst_rear;
		lst_rear = tmp;
	}
	dvprint("%s: inserts done.\n", __FUNCTION__);
}

/* LSTITERSTR
** Iterates linked list and applies func f to node.content
** Returns the node content if the func f returns error code
** lst: ptr to node
** f: ptr to function
** UNPROTECTED
*/
char	*ft_lstiterstr_mem(t_mem_mgr *m, t_list *lst, int (*f)(t_mem_mgr *m,
			void *))
{
	if (lst == NULL)
		return (NULL);
	while (lst)
	{
		if (0 != f(m, lst->content))
			return (lst->content);
		lst = lst->next;
	}
	return (NULL);
}
