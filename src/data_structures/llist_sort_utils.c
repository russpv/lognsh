#include "llist_int.h"

int    compare(t_list *beg, t_list *end)
{
	int res;

	res = ft_strcmp(beg->content, end->content);
    if (res > 0)
        return (AMORETHANB);
    else if (res < 0)
        return (ALESSTHANB);
    else
        return (0);
}

void	init_merge(struct s_merge *m, t_list **beg, t_list **end, t_list *mid)
{
	m->i = 0;
	m->tail = beg;
	m->tmp = NULL;
    m->l = *beg;
    m->r = mid->next;
	m->tmp2 = &(*((*end)->next)); // need this to reconnect segment with rest of list
	m->nodec = ft_lstsize_betw(*beg, *end);
	m->mid = mid;
	m->beg = beg;
	m->end = end;
}
