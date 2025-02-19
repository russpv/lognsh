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
	debug_print( "get midptr: ");
	ft_lstprint_betw(lst, end);
	assert(tmp != NULL && end != NULL);
	assert(tmp->next != NULL);
	assert(tmp->next->next != NULL);
	if (NULL == tmp || NULL == tmp->next)
        return (lst);
    if (NULL == tmp->next->next)
        return (end);
    while (tmp != NULL && tmp->next != NULL && tmp->next->next != NULL && tmp->next->next <= end )
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
	debug_print( "merge got l: \n");
	ft_lstprint_betw(*beg, mid);
	debug_print( "...and r: \n");
	ft_lstprint_betw(mid->next, *end);
	debug_print( "head: %s, nodecount:%d\n", (*lst)->content, nodec);
    if (ALESSTHANB == _compare(mid, mid->next, cfunc))
	{
		debug_print( "already sorted\n");
        return (*lst);
	}
    l = *beg;
    r = mid->next;
	debug_print( "merging:...\n");
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
		else
			debug_print( RED"WTF COMPARO EQUAL?! %s=%s\n"RESET, l->content, r->content);
		i++;
		if ((*tail) != NULL && tmp != NULL) //subsequent swaps can be the head ptr with no prev!
		{
			if (NULL != (*tail)->prev)
				debug_print(BLUE" -reassign prev %s to:%s- "RESET, (*tail)->prev->content, tmp->content);
			else
				debug_print(BLUE" -reassign NULL prev to:%s- "RESET, tmp->content);
			(*tail)->prev = tmp;
			if (NULL != (*tail)->prev && NULL != (*tail)->prev->prev)
				debug_print( MAGENTA" prev now linked %s<-%s->%s"RESET") -> \n", \
					(*tail)->prev->prev->content, (*tail)->prev->content, (*tail)->prev->next->content);
			else 
				debug_print( MAGENTA" prev now linked %p<-%s->%s"RESET") -> \n", \
					(*tail)->prev->prev, (*tail)->prev->content, (*tail)->prev->next->content);
		}
		if ((*tail) == *lst){
			debug_print( "%s (head) "GREEN"(prev: %p, next: %s), -> \n"RESET, (*tail)->content, (*tail)->prev, (*tail)->next->content);
			// set prev to NULL
		}
		else if ((*tail)->next)
			debug_print( "%s "GREEN"(prev: %s, next: %s), -> \n"RESET, (*tail)->content, (*tail)->prev->content, (*tail)->next->content);
		else
			debug_print( "%s "GREEN"(prev: %s, next: END), -> \n"RESET, (*tail)->content, (*tail)->prev->content);

		tmp = *tail;
		tail = &((*tail)->next);
		if (NULL != (*tail) && NULL != r)
		{
			if (NULL != (*end)->next && NULL != r->next)
				debug_print( BLUE"(tail now at: %s | l at: %s, l->next: %s, mid: %s, mid->next: %s "\
				"| r at: %s, r->next: %s, end: %s, end->next: %s "\
				"| beg: %s) \n"RESET,\
				(*tail)->content, l->content, l->next->content, mid->content, mid->next->content, \
				r->content, r->next->content, (*end)->content, (*end)->next->content, \
				(*beg)->content);
			else if (NULL == (*end)->next && NULL != r->next)
				debug_print( BLUE"(tail now at: %s | l at: %s, l->next: %s, mid: %s, mid->next: %s "\
				"| r at: %s, r->next: %s, end: %s, end->next: (END)) "\
				"| beg: %s) \n"RESET,\
				(*tail)->content, l->content, l->next->content, mid->content, mid->next->content, \
				r->content, r->next->content, (*end)->content, \
				(*beg)->content);
			else if (NULL == (*end)->next)
				debug_print( BLUE"(tail now at: %s | l at: %s, l->next: %s, mid: %s, mid->next: %s "\
				"| r at: (END), r->next: (END), end: %s, end->next: (END)) "\
				"| beg: %s) \n"RESET,\
				(*tail)->content, l->content, l->next->content, mid->content, mid->next->content, \
				(*end)->content, \
				(*beg)->content);
			else if (NULL == (*tail))
				debug_print( BLUE"(tail now at: END | l at: %s, l->next: %s, mid: %s, mid->next: %s "\
				"| r at: (END), r->next: (END), end: %s, end->next: (END)) "\
				"| beg: %s) \n"RESET,\
				l->content, l->next->content, mid->content, mid->next->content, \
				(*end)->content, \
				(*beg)->content);
			else
				debug_print( "WTF?!\n");
		}
	}
	if (*tail && (*end) && (*end)->next && tmp)
		debug_print( "end->next %s, tail:%s tmp:%s\n", (*end)->next->content, (*tail)->content, tmp->content);
	debug_print( "Adding remainders...\n");
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
	if ((*tail)->next && (*end)->next && tmp2)
		debug_print( BLUE"status:end->next %s, tail:%s tail->next: %s, tmp2:%s\n"RESET, (*end)->next->content, (*tail)->content,\
		(*tail)->next->content, (tmp2)->content);
	debug_print( "Advancing tail to end to terminate with tmp2 (i=%d)...\n", i);
	while (++i < nodec)
		tail = &((*tail)->next);
	// when on last node:
	if (*tail && (*tail)->next && (*end)->content && tmp2)
		debug_print( BLUE"status: end->next %s, tail:%s tail->next: %s, tmp2:%s\n"RESET, (*end)->next->content, (*tail)->content,\
		(*tail)->next->content, (tmp2)->content);
	if (*tail)
		(*tail)->next = tmp2;
	if (*tail && tmp2)
		debug_print( "rejoined to rest of list %s->%s\n", (*tail)->content, tmp2->content);
	if (*tail && (*end)->next && tmp2)
		debug_print( BLUE"status: end->next %s, tail:%s tail->next: %s, tmp2:%s\n"RESET, (*end)->next->content, (*tail)->content,\
		(*tail)->next->content, (tmp2)->content);
	// if tail refers back onto the sorted list, 
	if (*tail)
		(*end) = (*tail); // make sure end persists after changes.
	if (*tail)
		debug_print( "%s (remainder, "MAGENTA"prev:%s->%s) -> "RESET, (*tail)->content, (*tail)->prev->content, (*tail)->prev->next->content);
	if ((*end)->next == NULL && *tail == *end)
		(*tail)->next = NULL;
	(*lst)->prev = NULL;
	debug_print( "DONE:\n");
	if (*tail && (*tail)->next)
		debug_print( "|beg->next...tail->next : %s->%s...%s->%s|\n", (*beg)->content, (*beg)->next->content, (*tail)->content, (*tail)->next->content);
	ft_lstprint_betw(*beg, *tail);
	ft_lstprint_betw(*beg, *end);
	debug_print( "RETURNING head %s (%p<- ->%s). \n", (*lst)->content, (*lst)->prev, (*lst)->next->content);
	fflush(stderr);
	return (*lst);
}

static void    _solve(t_list **lst, t_list **beg, t_list **end, int (*cfunc)(const char *, const char *))
{
    t_list *mid;

    if (*beg == NULL || *beg == *end) {
		debug_print("_solve: NULL or one node, skipping\n");
        return ;
	}
    if ((*beg)->next == *end)
    {
		debug_print("_solve: two nodes, compare and swapping\n");
        if (AMORETHANB == _compare((*beg), (*end), cfunc))
            _swap((*beg), (*end));
        return ;
    }
	debug_print( "_solve: end=%s\n", (*end)->content);
    mid = _get_midptr(*beg, *end);
	if (*beg != mid) {
		debug_print(RED"Splitting first half: b:%s m:%s\n"RESET, (*beg)->content, mid->content);
    	_solve(lst, beg, &mid, cfunc);
	}
	if (mid->next != *end) {
		debug_print(RED"Splitting latter half: b:%s m:%s\n"RESET, mid->next->content, (*end)->content);
    	_solve(lst, &(mid->next), end, cfunc);
	}
	debug_print( "Solve got: ");
	ft_lstprint(*lst);
	if (*beg != mid && *beg != *end && (*beg)->next != *end)
    	*lst = _merge(lst, beg, mid, end, cfunc);
}

// Sorts llist in ascending lexicographic order. 
t_list *ft_lstsort(t_list **lst, int (*cfunc)(const char *, const char *))
{
    t_list *last;

    if (!lst)
        return ((t_list *)-1);
    if (!*lst)
        return ((t_list *)-1);
    last = ft_lstlast(*lst);
	debug_print( "Sort got: ");
	ft_lstprint(*lst);
    _solve(lst, lst, &last, cfunc);
    return (*lst);
}
