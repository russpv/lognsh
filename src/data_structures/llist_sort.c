#include "llist_int.h"

bool	str_isalnum(const char *s)
{
	if (!s)
		return (false);
	while (*s)
	{
		if (!ft_isalnum(*s))
			return (false);
		s++;
	}
	return (true);
}

bool	str_hasalnum(const char *s)
{
	if (!s)
		return (false);
	while (*s)
	{
		if (ft_isalnum(*s))
			return (true);
		s++;
	}
	return (false);
}

void	init_merge(struct s_merge *m, t_list **beg, t_list **end, t_list *mid)
{
	m->i = 0;
	m->tail = beg;
	m->tmp = NULL;
	m->l = *beg;
	m->r = mid->next;
	m->tmp2 = &(*((*end)->next));
	m->nodec = ft_lstsize_betw(*beg, *end);
	m->mid = mid;
	m->beg = beg;
	m->end = end;
}

static int	_swap(t_list *beg, t_list *end)
{
	void	*tmp;

	tmp = beg->content;
	beg->content = end->content;
	end->content = tmp;
	return (0);
}

// If 1 node, mid is beg
// If 2 nodes, mid is end
// If 3 nodes, mid is the middle
static t_list	*_get_midptr(t_list *lst, t_list *end)
{
	t_list	*mid;
	t_list	*tmp;

	mid = lst;
	tmp = lst;
	ft_lstprint_betw(lst, end);
	if (NULL == tmp || NULL == tmp->next)
		return (lst);
	if (NULL == tmp->next->next)
		return (end);
	while ((tmp != NULL && tmp->next != NULL) && (tmp != end
			&& tmp->next != end))
	{
		mid = mid->next;
		tmp = tmp->next->next;
	}
	return (mid);
}

// Pass double pointers so modifications persist across recursive calls
static void	_solve(t_list **lst, t_list **beg, t_list **end)
{
	t_list	*mid;

	if (*beg == NULL || *beg == *end)
		return ;
	if ((*beg)->next == *end)
	{
		if (AMORETHANB == compare((*beg), (*end)))
			_swap((*beg), (*end));
		return (dprint("_solve: compare/swapped two nodes.\n"));
	}
	mid = _get_midptr(*beg, *end);
	if (*beg != mid)
	{
		dprint(RED "Splitting first half: b:%s m:%s\n" RESET,
			(*beg)->content, mid->content);
		_solve(lst, beg, &mid);
	}
	if (mid->next != *end)
	{
		dprint(RED "Splitting latter half: b:%s m:%s\n" RESET,
			mid->next->content, (*end)->content);
		_solve(lst, &(mid->next), end);
	}
	if (*beg != mid && *beg != *end && (*beg)->next != *end)
		merge(lst, beg, mid, end);
}

// Sorts a content->string llist in CUSTOM ascending lexicographic order.
// Operator chars are superior. Any '$' prefixed string is superior.
t_list	*ft_lstsort(t_list **lst)
{
	t_list	*last;

	if (!lst)
		return ((t_list *)-1);
	if (!*lst)
		return ((t_list *)-1);
	last = ft_lstlast(*lst);
	dprint("%s: got: ", __FUNCTION__);
	ft_lstprint(*lst);
	_solve(lst, lst, &last);
	dprint("%s: returning: ", __FUNCTION__);
	ft_lstprint(*lst);
	return (*lst);
}
