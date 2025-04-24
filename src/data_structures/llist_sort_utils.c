
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
