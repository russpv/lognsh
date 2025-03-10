#include "state_int.h"

#define DMSG_GOT "%p got lst = %p, head = %p\n"
#define DMSG_DEL "%p deleting node = %p\n"


void	set_parser(t_state *state, t_parser *p)
{
	state->current_parser = p;
}

void	set_lexer(t_state *state, t_lex *l)
{
	state->current_lexer = l;
}

void	set_input(t_state *s, char *input)
{
	s->input = input;
}

void	set_command(t_state *s, t_cmd *c)
{
	s->current_cmd = c;
}

int	set_exit_status(t_state *state, int value)
{
	state->current_exit_code = value;
	return (0);
}

void	set_tmp(t_state *s, char *str)
{
	if (s->tmp)
		free(s->tmp);
	s->tmp = ft_strdup(str);
}

void	*myalloc(t_state *s, size_t size)
{
	void	*new;

	if (size < 1)
		return (NULL);
	new = malloc(size);
	if (new)
	{
		if (0 != mem_add_mem(&s->mem_list, new, size))
		{
			mem_free_all(&s->mem_list);
			perror("Memory allocation failed.\n");
			exit(ENOMEM);
		}
	}
	return (new);
}

// TODO add mem_rm_block(...);
void	myfree(t_state *s, void *alloc)
{
	t_mem_block	*block;

	if (!s || !alloc)
		return ;
	block = mem_get_alloc(&s->mem_list, alloc);
	if (!block)
	{
		err("Memory corruption or double free detected! (maybe, WIP)\n");
		free(alloc); // TODO remove after migration
		return ;
	}
	if (true == block->is_freed)
		return (err("Double free detected.\n"));
	block->is_freed = true;
	free(alloc);
}
/* LSTNEW
** Returns new linked list head ptr with one node
** But content is not copied
*/
t_list	*ft_lstnew_tmp(t_state *s, void *content)
{
	t_list	*ll;

	ll = myalloc(s, sizeof(t_list));
	if (!ll)
		return (NULL);
	ll->next = NULL;
	ll->prev = NULL;
	ll->content = content;
	return (ll);
}

/* LSTDELONE
** Removes passed node in doubly linked list
** Assuming head to rear iteration direction
** lst: parent list
** node: node to free
** del: ptr to func that deletes node.content
*/
void	ft_lstdelone_tmp(t_state *s, t_list **lst, t_list *node, void (*del)(t_state *s, void *))
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
	del(s, node->content);
	myfree(s, node);
}

/* LSTCLEAR
** Deletes all subsequent nodes and current node
** lst: ptr to ptr to node
** del: ptr to free(content) function
*/
void	ft_lstclear_tmp(t_state *s, t_list **lst, void (*del)(t_state *s, void *))
{
	t_list	*tmp;
	t_list	*next;

	if (!*lst || !del || !lst)
		return ;
	debug_print(DMSG_GOT, __FUNCTION__, (void *)lst, (void *)*lst);
	tmp = *lst;
	while (tmp)
	{
		debug_print(DMSG_DEL, __FUNCTION__, (void *)tmp);
		next = tmp->next;
		ft_lstdelone_tmp(s, lst, tmp, del);
		tmp = next;
	}
	*lst = NULL;
}
// Helper function: Deep copy a single node's content
t_list	*ft_lstcopy_node_tmp(t_state *s, const t_list *orig, void *(*f)(t_state *s, const void *))
{
	t_list	*new_node;

	new_node = malloc(sizeof(t_list));
	if (!new_node)
		return (NULL);
	new_node->content = f(s, orig->content);
	if (!new_node->content)
	{
		free(new_node);
		return (NULL);
	}
	new_node->next = NULL;
	new_node->prev = NULL;
	return (new_node);
}

t_list	*ft_lstcopy_tmp(t_state *s, t_list *orig, void *(*f)(t_state *s, const void *), void (*d)(t_state *s, void *))
{
	t_list	*new_list;
	t_list	*prev_node;
	t_list	*new_node;

	new_list = NULL;
	prev_node = NULL;
	while (orig)
	{
		new_node = ft_lstcopy_node_tmp(s, orig, f);
		if (!new_node)
		{
			ft_lstclear_tmp(s, &new_list, d);
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

#define DMSG_IN "%s: Deleting node: %p (content: %p) (head: %p)\n"
#define DMSG_OUT "%s: Node deleted successfully\n"

/* LSTDELONE RWD
** Removes passed node in doubly linked list
** Assuming REAR to HEAD iteration direction
** lst: parent list
** node: node to free
** del: ptr to func that deletes node.content
** Handles case where same pointer passed in arg 1 and 2
*/
void   ft_lstdelone_rwd_tmp(t_state *s, t_list **lst, t_list **node, void (*del)(t_state *s, void *))
{
      t_list  *tmp;

      if (!lst || !node || !(*node) || !del)
              return ;
      debug_print(DMSG_IN, __FUNCTION__,
              (void *)(*node), (void *)((*node)->content), (void *)lst);
      tmp = (*node)->prev;
      if ((*node)->prev)
              (*node)->prev->next = (*node)->next;
      if ((*node)->next)
              (*node)->next->prev = (*node)->prev;
      if ((*node)->content)
              del(s, (*node)->content);
      free(*node);
      *node = NULL;
      if (*lst == *node)
              *lst = tmp;
      debug_print(DMSG_OUT, __FUNCTION__);
}

/*LSTMAP
** Returns new (doubly) linked list head ptr using func f
** on each node of an existing list.
** lst: ptr to node
** f: function applied to each node.content
** del: used if allocation fails
** UNPROTECTED: returns NULL only when !lst
*/
t_list	*ft_lstmap_tmp(t_state *s, t_list *lst, void *(*f)(void *), void (*del)(t_state *s, void *))
{
	t_list	*newlst;
	t_list	*new;
	void	*new_content;

	newlst = NULL;
	if (!lst)
		return (NULL);
	while (lst)
	{
		new_content = f(lst->content);
		new = ft_lstnew(new_content);
		if (!new)
		{
			(*del)(s, new_content);
			if (newlst)
			{
				ft_lstclear_tmp(s, &newlst, del);
				newlst = NULL;
			}
			return (err("Malloc"), NULL);
		}
		ft_lstadd_back(&newlst, new);
		lst = lst->next;
	}
	return (newlst);
}
#define DBGMSG_LSTINSRWD_ANNOUNCE "lstiter_ins_rwd: got lst: %p, head:%p.\n"
#define DBGMSG_LSTINSRWD_NULLARG "lstiter_ins_rwd: NULL input.\n"
#define DBGMSG_LSTINSRWD_ENDNODE "lstiter_ins_rwd: Starting from end: %p\n"
#define DBGMSG_LSTINSRWD_PREF "lstiter_ins_rwd: Iterating node: %p, content: %p\n"
#define DBGMSG_LSTINSRWD_POSTF "lstiter_ins_rwd: Function f applied to node: %p\n"
#define DBGMSG_LSTINSRWD_SUCCESS "lstiter_ins_rwd: ft_lstiter_ins_rwd Iteration complete.\n"

/* Iterates BACKWARDS and applies f */
void	ft_lstiter_ins_rwd_tmp(t_state *s, t_list **lst, int (*f)(t_state *s, t_list **, void *))
{
	t_list	*lst_rear;
	t_list	*tmp;

	if (lst == NULL)
		return (debug_print(DBGMSG_LSTINSRWD_NULLARG));
	lst_rear = ft_lstlast(*lst);
	debug_print(DBGMSG_LSTINSRWD_ENDNODE, (void *)lst_rear);
	while (lst_rear)
	{
		tmp = lst_rear->prev;
		debug_print(DBGMSG_LSTINSRWD_PREF,(void *)lst_rear, \
					(void *)lst_rear->content);
		f(s, &(lst_rear), lst_rear->content);
		debug_print(DBGMSG_LSTINSRWD_POSTF, (void *)lst_rear);
		if (NULL == tmp)
			*lst = lst_rear;
		lst_rear = tmp;
	}
	debug_print(DBGMSG_LSTINSRWD_SUCCESS);
}