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
	debug_print(DMSG_GOT, __FUNCTION__, (void *)lst, (void *)*lst);
	tmp = *lst;
	while (tmp)
	{
		debug_print(DMSG_DEL, __FUNCTION__, (void *)tmp);
		next = tmp->next;
		ft_lstdelone_tmp(mgr, lst, tmp, del);
		tmp = next;
	}
	*lst = NULL;
}

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
	debug_print(DMSG_GOT, __FUNCTION__, (void *)lst, (void *)*lst);
	tmp = *lst;
	while (tmp)
	{
		debug_print(DMSG_DEL, __FUNCTION__, (void *)tmp);
		next = tmp->next;
		ft_lstdelone_str_tmp(mgr, lst, tmp);
		tmp = next;
	}
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

#define DMSG_IN "%s: Deleting node: %p (content: %p) (list: %p)\n"
#define DMSG_OUT "%s: Node deleted successfully\n"

/* LSTDELONE RWD
** Removes passed node in doubly linked list
** Assuming REAR to list iteration direction
** lst: parent list
** node: node to free
** del: ptr to func that deletes node.content
** Handles case where same pointer passed in arg 1 and 2
*/
void	ft_lstdelone_rwd_tmp(t_mem_mgr *mgr, t_list **lst, t_list **node,
		void (*del)(t_mem_mgr *, void **))
{
	t_list	*tmp;

	if (!mgr || !lst || !node || !(*node))
		return ;
	debug_print(DMSG_IN, __FUNCTION__, (void *)(*node),
		(void *)((*node)->content), (void *)lst);
	tmp = (*node)->next;
	if ((*node)->prev)
		(*node)->prev->next = (*node)->next;
	if ((*node)->next)
		(*node)->next->prev = (*node)->prev;
	if ((*node)->content)
		del(mgr, &(*node)->content);
	if (*lst == *node)
		*lst = tmp;
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
#define DBGMSG_LSTINSRWD_SUCCESS "%s: Iteration complete.\n"

/* Iterates BACKWARDS and applies insertion func f */
void	ft_lstiter_ins_rwd_tmp(t_mem_mgr *mgr, t_list **lst,
		int (*f)(t_mem_mgr *, t_list **, void *))
{
	t_list	*lst_rear;
	t_list	*tmp;

	if (lst == NULL)
		return (debug_print(DBGMSG_LSTINSRWD_NULLARG, __FUNCTION__));
	lst_rear = ft_lstlast(*lst);
	debug_print(DBGMSG_LSTINSRWD_ENDNODE, __FUNCTION__, (void *)lst_rear);
	while (lst_rear)
	{
		tmp = lst_rear->prev;
		debug_print(DBGMSG_LSTINSRWD_PREF, __FUNCTION__, (void *)lst_rear,
			(void *)lst_rear->content);
		f(mgr, &(lst_rear), lst_rear->content);
		debug_print(DBGMSG_LSTINSRWD_POSTF, __FUNCTION__, (void *)lst_rear);
		if (NULL == tmp)
			*lst = lst_rear;
		lst_rear = tmp;
	}
	debug_print(DBGMSG_LSTINSRWD_SUCCESS, __FUNCTION__);
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
