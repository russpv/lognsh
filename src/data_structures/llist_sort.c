# include "llist_int.h"

static int  _swap(t_list *beg, t_list *end)
{
    void *tmp; 

    tmp = beg->content;
    beg->content = end->content;
    end->content = tmp;
	return (0);
}

// If 1 node, mid is beg
// If 2 nodes, mid is end
// If 3 nodes, mid is the middle
static t_list  *_get_midptr(t_list *lst, t_list *end)
{
    t_list *mid;
    t_list *tmp;

    mid = lst;
    tmp = lst;
	ft_lstprint_betw(lst, end);
	assert(tmp != NULL && end != NULL);
	assert(tmp->next != NULL);
	assert(tmp->next->next != NULL);
	if (NULL == tmp || NULL == tmp->next)
        return (lst);
    if (NULL == tmp->next->next) // don't want the last node of a pair to be mid
        return (end);
    while ((tmp != NULL && tmp->next != NULL) && (tmp != end && tmp->next != end))
    {
        mid = mid->next;
        tmp = tmp->next->next;
    }
    return (mid);
}

// Pass double pointers so modifications persist across recursive calls
static void    _solve(t_list **lst, t_list **beg, t_list **end)
{
    t_list *mid;

    if (*beg == NULL || *beg == *end)
        return ;
    if ((*beg)->next == *end)
    {
        if (AMORETHANB == compare((*beg), (*end)))
            _swap((*beg), (*end));
        return (debug_print("_solve: compare/swapped two nodes.\n"));
    }

    mid = _get_midptr(*beg, *end);

	if (*beg != mid) 
	{
		debug_print(RED"Splitting first half: b:%s m:%s\n"RESET, (*beg)->content, mid->content);
    	_solve(lst, beg, &mid);
	}
	if (mid->next != *end) 
	{
		debug_print(RED"Splitting latter half: b:%s m:%s\n"RESET, mid->next->content, (*end)->content);
    	_solve(lst, &(mid->next), end);
	}
	if (*beg != mid && *beg != *end && (*beg)->next != *end)
    	merge(lst, beg, mid, end);
}

// Sorts a content->string llist in ascending lexicographic order. 
t_list *ft_lstsort(t_list **lst)
{
    t_list *last;

    if (!lst)
        return ((t_list *)-1);
    if (!*lst)
        return ((t_list *)-1);
    last = ft_lstlast(*lst);
	debug_print( "%s: got: ", __FUNCTION__);
	ft_lstprint(*lst);
    _solve(lst, lst, &last);
	debug_print( "%s: returning: ", __FUNCTION__);
	ft_lstprint(*lst);
    return (*lst);
}
