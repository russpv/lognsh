# include "llist.h"

# define ALESSTHANB -1
# define AMORETHANB 1

static int  _swap(t_list *beg, t_list *end)
{
    void *tmp; 

    tmp = beg->content;
    beg->content = end->content;
    end->content = tmp;
	return (0);
}

static int    _compare(t_list *beg, t_list *end, int (*cfunc)(const char *, const char *))
{
	int res;

	res = cfunc(beg->content, end->content);
    if (res > 0)
        return (AMORETHANB);
    else if (res < 0)
        return (ALESSTHANB);
    else
        return (0);
}
// If 1 node, mid is beg
// If 2 nodes, mid is end
// If 3 nodes, mid is the middle
t_list  *_get_midptr(t_list *lst, t_list *end)
{
    t_list *mid;
    t_list *tmp;

    mid = lst;
    tmp = lst;
	//fprintf(stderr, "get midptr: ");
	//ft_lstprint_betw(lst, end);
    if (NULL == tmp || NULL == tmp->next)
        return (lst);
    if (NULL == tmp->next->next)
        return (end);
    while (tmp != NULL && tmp->next->next <= end)
    {
        mid = mid->next;
        tmp = tmp->next->next;
    }
    return (mid);
}

// TODO: when the real head changes, need to update the prev to NULL
// TODO: there's a problem when l gets rearranged and mid->next changes, never breaks out of loop
// update the debugging to see the changes happening, probably need another placeholder good god
static t_list	*_merge(t_list **lst, t_list **beg, t_list *mid, t_list **end, int (*cfunc)(const char *, const char *))
{
    t_list *l;
    t_list *r;
    t_list *tmp;
    t_list **tail; //  = malloc(sizeof(t_list **));
	t_list *tmp2;

	const int nodec = ft_lstsize_betw(*beg, *end);
	int i;

	i = 0;
	tail = beg;
	tmp = NULL;
	fprintf(stderr, "merge got l: \n");
	ft_lstprint_betw(*beg, mid);
	fprintf(stderr, "...and r: \n");
	ft_lstprint_betw(mid->next, *end);
    if (ALESSTHANB == _compare(mid, mid->next, cfunc))
	{
		fprintf(stderr, "already sorted\n");
        return (*lst);
	}
    l = *beg;
    r = mid->next;
	fprintf(stderr, "merging:...\n");
    while (NULL != l && NULL != r && mid->next != l && (*end)->next != r && i <= nodec)
    {
        if (ALESSTHANB == _compare(l, r, cfunc))
        {
			*tail = l;
			l = l->next;
		}
    	else if (AMORETHANB == _compare(l, r, cfunc))
		{
			*tail = r;
			r = r->next;
		}
		i++;
		if ((*tail) != NULL && tmp != NULL)
		{
			fprintf(stderr,BLUE" -reassignin prev to:%s- "RESET, tmp->content);
			(*tail)->prev = tmp;
			fprintf(stderr, MAGENTA" prev now linked %s->%s"RESET") -> \n", (*tail)->prev->content, (*tail)->prev->next->content);
		}
		fprintf(stderr, "%s "GREEN"(prev: %s, next: %s), -> \n"RESET, (*tail)->content, (*tail)->prev->content, (*tail)->next->content);
		tmp = *tail;
		tail = &((*tail)->next);
		fprintf(stderr, BLUE"(tail now at: %s, l at: %s, r at: %s, end: %s, end->next: %s, beg: %s) \n"RESET, (*tail)->content, l->content, r->content, (*end)->content, (*end)->next->content, (*beg)->content);
	}
	fprintf(stderr, "end->next %s, tail:%s tmp:%s\n", (*end)->next->content, (*tail)->content, tmp->content);
	tmp2 = (*end)->next;
	if (l != mid->next)
	{
		*tail = l;
		(*tail)->prev = tmp;
	}
	else if (r != (*end)->next)
	{
		*tail = r;
		(*tail)->prev = tmp;
	}
	fprintf(stderr, "end->next %s, tail:%s tmp2:%s\n", (*end)->next->content, (*tail)->content, (tmp2)->content);
	while (i++ != nodec)
		tail = &((*tail)->next);
	// when on last node:
	(*tail)->next = tmp2;
	fprintf(stderr, "rejoining rest of list %s->%s\n", (*tail)->content, tmp2->content);
	(*end) = (*tail); // make sure end persists after changes.
	fprintf(stderr, "%s (remainder, "MAGENTA"prev:%s->%s) -> "RESET, (*tail)->content, (*tail)->prev->content, (*tail)->prev->next->content);
	if ((*end)->next == NULL && *tail == *end)
		(*tail)->next = NULL;
	fprintf(stderr, "DONE:\n");
	fprintf(stderr, "|beg->next...tail->next : %s->%s...%s->%s|\n", (*beg)->content, (*beg)->next->content, (*tail)->content, (*tail)->next->content);
	ft_lstprint_betw(*beg, *tail);
	ft_lstprint_betw(*beg, *end);
	fprintf(stderr, "RETURNING head %s. \n", (*lst)->content);
	fflush(stderr);
	getchar();
	return (*lst);
}

static void    _solve(t_list **lst, t_list **beg, t_list **end, int (*cfunc)(const char *, const char *))
{
    t_list *mid;

    if (*beg == NULL || *beg == *end)
        return ;
    if ((*beg)->next == *end)
    {
        if (AMORETHANB == _compare((*beg), (*end), cfunc))
            _swap((*beg), (*end));
        return ;
    }
    mid = _get_midptr(*beg, *end);
	if (*beg != mid) {
		fprintf(stderr,RED"Splitting first half: b:%s m:%s\n"RESET, (*beg)->content, mid->content);
    	_solve(lst, beg, &mid, cfunc);
	}
	if (mid->next != *end) {
		fprintf(stderr,RED"Splitting latter half: b:%s m:%s\n"RESET, mid->next->content, (*end)->content);
    	_solve(lst, &(mid->next), end, cfunc);
	}
	fprintf(stderr, "Solve got: ");
	ft_lstprint(*lst);
	if (*beg != mid && *beg != *end && (*beg)->next != *end)
    	*lst = _merge(lst, beg, mid, end, cfunc);
}

/* Sorts llist in ascending lexicographic order. */
t_list *ft_lstsort(t_list **lst, int (*cfunc)(const char *, const char *))
{
    t_list *last;

    if (!lst)
        return ((t_list *)-1);
    if (!*lst)
        return ((t_list *)-1);
    last = ft_lstlast(*lst);
	fprintf(stderr, "Sort got: ");
	ft_lstprint(*lst);
    _solve(lst, lst, &last, cfunc);
    return (*lst);
}
