#include "llist.h"

/* LSTITER
** Iterates linked list and applies func f to node.content
** lst: ptr to node
** f: ptr to function
** UNPROTECTED
*/
void	ft_lstiter(t_list *lst, void (*f)(void *))
{
	if (lst == NULL)
		return ;
	while (lst)
	{
		f((lst)->content);
		lst = (lst)->next;
	}
}

/* Iterates BACKWARDS and applies f */
void	ft_lstiter_ins_rwd(t_list **lst, int (*f)(t_list **, void *))
{
	t_list	*lst_rear;
	t_list	*tmp;

	debug_print("lstiter_ins_rwd: got lst: %p, head:%p.\n", (void *)lst,
		(void *)*lst);
	if (lst == NULL)
		return (debug_print("lstiter_ins_rwd: NULL input.\n"));
	lst_rear = ft_lstlast(*lst);
	debug_print("lstiter_ins_rwd: Starting from end: %p\n", (void *)lst_rear);
	while (lst_rear)
	{
		tmp = lst_rear->prev;
		debug_print("lstiter_ins_rwd: Iterating node: %p, content: %p\n",
			(void *)lst_rear, (void *)lst_rear->content);
		f(&(lst_rear), lst_rear->content);
		debug_print("lstiter_ins_rwd: Function f applied to node: %p\n",
			(void *)lst_rear);
		if (NULL == tmp)
			*lst = lst_rear;
		lst_rear = tmp;
	}
	debug_print("lstiter_ins_rwd: ft_lstiter_ins_rwd Iteration complete.\n");
}

/* LSTITERSTR
** Iterates linked list and applies func f to node.content
** Returns the node content if the func f returns error code
** lst: ptr to node
** f: ptr to function
** UNPROTECTED
*/
char	*ft_lstiterstr(t_list *lst, int (*f)(void *))
{
	if (lst == NULL)
		return (NULL);
	while (lst)
	{
		if (0 != f(lst->content))
			return (lst->content);
		lst = lst->next;
	}
	return (NULL);
}
