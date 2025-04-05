/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   i_llist_utils_1.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dayeo <dayeo@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/05 22:23:12 by dayeo             #+#    #+#             */
/*   Updated: 2025/04/05 22:23:37 by dayeo            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "i_llist.h"

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
	debugv_print(DMSG_GOT, __FUNCTION__, (void *)lst, (void *)*lst);
	tmp = *lst;
	while (tmp)
	{
		debugv_print(DMSG_DEL, __FUNCTION__, (void *)tmp);
		next = tmp->next;
		ft_lstdelone_str_tmp(mgr, lst, tmp);
		tmp = next;
	}
	*lst = NULL;
	debug_print("%s: Lst deleted successfully\n", __FUNCTION__);
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
