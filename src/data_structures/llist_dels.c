#include "llist_int.h"

#define DEBUGMSG_DRWD_ANNOUNCE "lstdelone_rwd: Deleting node: %p (content: %p) (head: %p)\n"
#define DEBUGMSG_DRWD_SUCCESS "lstdelone_rwd: Node deleted successfully\n"

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
void   ft_lstdelone_rwd(t_list **lst, t_list **node, void (*del)(void *))
{
      t_list  *tmp;

      if (!lst || !node || !(*node) || !del)
              return ;
      debug_print(DEBUGMSG_DRWD_ANNOUNCE,
              (void *)(*node), (void *)((*node)->content), (void *)lst);
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
      debug_print(DEBUGMSG_DRWD_SUCCESS);
}
