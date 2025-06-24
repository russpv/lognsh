#include "llist_int.h"

// Helper function: Deep copy a single node's content
t_list	*ft_lstcopy_node(const t_list *orig, void *(*f)(const void *))
{
	t_list	*new_node;

	new_node = malloc(sizeof(t_list));
	if (!new_node)
		return (NULL);
	new_node->content = f(orig->content);
	if (!new_node->content)
	{
		free(new_node);
		return (NULL);
	}
	new_node->next = NULL;
	new_node->prev = NULL;
	return (new_node);
}

/* Deep copies linked lists
 * f - content deep copy func; d - content delete func
 */
t_list	*ft_lstcopy(t_list *orig, void *(*f)(const void *), void (*d)(void *))
{
	t_list	*new_list;
	t_list	*prev_node;
	t_list	*new_node;

	new_list = NULL;
	prev_node = NULL;
	while (orig)
	{
		new_node = ft_lstcopy_node(orig, f);
		if (!new_node)
		{
			ft_lstclear(&new_list, d);
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


/* -------------------------------------------------------------------------
 * Function Group: linked list delete operations
 * Description   : 
 * ------------------------------------------------------------------------- */

#define MSG_IN "%s: deleting node: %p (content:%p head: %p)\n"

/* LSTDELONE
** Removes passed node in doubly linked list
** Assuming head to rear iteration direction
** lst: parent list
** node: node to free
** del: ptr to func that deletes node.content
*/
void	ft_lstdelone(t_list **lst, t_list *node, void (*del)(void *))
{
	if (!lst || !node || !del)
		return ;
	if (!*lst)
		return ;
	if (node->prev)
		node->prev->next = node->next;
	if (node->next)
		node->next->prev = node->prev;
	if (node == *lst)
		*lst = node->next;
	del(node->content);
	free(node);
}

/* LSTDELONE RWD
** Removes passed node in doubly linked list
** Assuming REAR to HEAD iteration direction
** lst: parent list
** node: node to free
** del: ptr to func that deletes node.content
** Handles case where same pointer passed in arg 1 and 2
*/
void	ft_lstdelone_rwd(t_list **lst, t_list **node, void (*del)(void *))
{
	t_list	*tmp;

	if (!lst || !node || !(*node) || !del)
		return ;
	dprint(MSG_IN, __FUNCTION__, (void *)(*node), (void *)((*node)->content),
		(void *)lst);
	tmp = (*node)->prev;
	if ((*node)->prev)
		(*node)->prev->next = (*node)->next;
	if ((*node)->next)
		(*node)->next->prev = (*node)->prev;
	if ((*node)->content)
		del((*node)->content);
	free(*node);
	*node = NULL;
	if (*lst == *node)
		*lst = tmp;
	dprint("%s: node deleted.\n", __FUNCTION__);
}
