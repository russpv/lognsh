#include "llist_int.h"

#define DBGMSG_LSTINSRWD_ANNOUNCE "lstiter_ins_rwd: got lst: %p, head:%p.\n"
#define DBGMSG_LSTINSRWD_NULLARG "lstiter_ins_rwd: NULL input.\n"
#define DBGMSG_LSTINSRWD_ENDNODE "lstiter_ins_rwd: Starting from end: %p\n"
#define DBGMSG_LSTINSRWD_PREF "lstiter_ins_rwd: Iterating node: %p, content:\
	%p\n"
#define DBGMSG_LSTINSRWD_POSTF "lstiter_ins_rwd: Function f applied to node:\
	%p\n"
#define DBGMSG_LSTINSRWD_SUCCESS "lstiter_ins_rwd: ft_lstiter_ins_rwd Iteration complete.\n"

/* LSTITER
** Iterates linked list and applies func f to node.content
** lst: ptr to node
** f: ptr to function
** UNPROTECTED
*/
int	ft_lstiter(t_list *lst, int (*f)(void *))
{
	if (lst == NULL)
		return (-1);
	while (lst)
	{
		if (0 != f((lst)->content))
			return (-1);
		lst = (lst)->next;
	}
	return (0);
}

/* Iterates BACKWARDS and applies f */
void	ft_lstiter_ins_rwd(t_list **lst, int (*f)(t_list **, void *))
{
	t_list	*lst_rear;
	t_list	*tmp;

	debug_print(DBGMSG_LSTINSRWD_ANNOUNCE, (void *)lst, (void *)*lst);
	if (lst == NULL)
		return (debug_print(DBGMSG_LSTINSRWD_NULLARG));
	lst_rear = ft_lstlast(*lst);
	debug_print(DBGMSG_LSTINSRWD_ENDNODE, (void *)lst_rear);
	while (lst_rear)
	{
		tmp = lst_rear->prev;
		debug_print(DBGMSG_LSTINSRWD_PREF, (void *)lst_rear,
			(void *)lst_rear->content);
		f(&(lst_rear), lst_rear->content);
		debug_print(DBGMSG_LSTINSRWD_POSTF, (void *)lst_rear);
		if (NULL == tmp)
			*lst = lst_rear;
		lst_rear = tmp;
	}
	debug_print(DBGMSG_LSTINSRWD_SUCCESS);
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
