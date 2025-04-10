/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   i_llist_str.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rpeavey <rpeavey@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/10 16:30:02 by rpeavey           #+#    #+#             */
/*   Updated: 2025/04/10 16:30:03 by rpeavey          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "i_llist.h"

/* LSTDELONE
** Removes passed node in doubly linked list
** Assuming list to rear iteration direction
** lst: parent list
** node: node to free
** del: ptr to func that deletes node.content
*/
void	ft_lstdelone_str_tmp(t_mem_mgr *m, t_list **lst, t_list *node)
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
	m->dealloc(&m->list, node->content);
	m->dealloc(&m->list, node);
}

/* LSTCLEAR
** Deletes all subsequent nodes and current node
** lst: ptr to ptr to node
** del: ptr to free(content) function
*/
void	ft_lstclear_str_tmp(t_mem_mgr *mgr, t_list **lst)
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
		ft_lstdelone_str_tmp(mgr, lst, tmp);
		tmp = next;
	}
	*lst = NULL;
	dvprint("%s: Lst deleted successfully\n", __FUNCTION__);
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

/* STRDUP
** This returns pointer to new string
** malloc handles errno
*/
char	*ft_strdup_tmp(t_mem_mgr *mgr, const char *s)
{
	char	*newstr;
	size_t	len;

	len = ft_strlen(s);
	newstr = (char *)(*mgr->f)(&mgr->list, sizeof(char) * len + 1);
	if (!newstr)
		return (NULL);
	newstr[len] = 0;
	return (ft_memcpy(newstr, s, len));
}
