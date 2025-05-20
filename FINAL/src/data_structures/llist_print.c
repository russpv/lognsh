#include "llist_int.h"

void	ft_lstprint(t_list *lst)
{
	int	count;

	count = 0;
	if (lst == NULL)
		return ;
	if (DEBUG)
	{
		cprintf(RED, "linkedl: " RESET);
		while (lst)
		{
			count++;
			fprintf(stderr, YELLOW "_%s_ -> " RESET,
				(const char *)lst->content);
			lst = lst->next;
		}
		cprintf(RED, "printed %d nodes.\n" RESET, count);
	}
	return ;
}

void	ft_lstprinter(t_list *lst, int (*f)(void *))
{
	int	count;

	count = 0;
	if (lst == NULL)
		return (cprintf(RED, "nothing to print.\n" RESET));
	if (DEBUG)
	{
		cprintf(RED, "linkedl: " RESET);
		while (lst)
		{
			count++;
			if (lst->content)
				f(lst->content);
			fprintf(stderr, YELLOW " -> " RESET);
			lst = lst->next;
		}
		cprintf(RED, "printed %d nodes.\n" RESET, count);
	}
	return ;
}

// Prints from lst to end
void	ft_lstprint_betw(t_list *lst, t_list *end)
{
	int	count;

	count = 0;
	if (lst == NULL || end == NULL)
		return ;
	if (DEBUG)
	{
		cprintf(RED, "ll segment: " RESET);
		while (lst != end->next)
		{
			count++;
			fprintf(stderr, YELLOW "%s -> " RESET, (const char *)lst->content);
			lst = lst->next;
		}
		cprintf(RED, "printed %d nodes.\n" RESET, count);
	}
	return ;
}


/* -------------------------------------------------------------------------
 * Function Group: linked list sort debug printers
 * Description   : 
 * ------------------------------------------------------------------------- */

static void	_print_one(struct s_merge *m)
{
	dvprint(BLUE "(tail now at: %s | l at: %s, l->next: %s, mid: %s,"
		"mid->next: %s | r at: %s, r->next: %s, end: %s, end->next: %s "
		"| beg: %s) \n" RESET,
		(*m->tail)->content,
		m->l->content,
		m->l->next->content,
		m->mid->content,
		m->mid->next->content,
		m->r->content,
		m->r->next->content,
		(*m->end)->content,
		(*m->end)->next->content,
		(*m->beg)->content);
}

static void	_print_two(struct s_merge *m)
{
	dvprint(BLUE "(tail now at: %s | l at: %s, l->next: %s, mid: %s,"
		"mid->next: %s | r at: %s, r->next: %s, end: %s, "
		"end->next: (end)) "
		"| beg: %s) \n" RESET,
		(*m->tail)->content,
		m->l->content,
		m->l->next->content,
		m->mid->content,
		m->mid->next->content,
		m->r->content,
		m->r->next->content,
		(*m->end)->content,
		(*m->beg)->content);
}

static void	_print_three(struct s_merge *m)
{
	dvprint(BLUE "(tail now at: %s | l at: %s, l->next: %s, mid: %s,"\
"mid->next: %s | r at: (end), r->next: (end), end: %s, "
		"end->next: (end)) "
		"| beg: %s) \n" RESET,
		(*m->tail)->content,
		m->l->content,
		m->l->next->content,
		m->mid->content,
		m->mid->next->content,
		(*m->end)->content,
		(*m->beg)->content);
}

static void	_print_four(struct s_merge *m)
{
	dvprint(BLUE "(tail now at: end | l at: %s, l->next: %s, mid: %s,"\
"mid->next: %s | r at: (end), r->next: (end), end: %s, "
		"end->next: (end)) "
		"| beg: %s) \n" RESET,
		m->l->content,
		m->l->next->content,
		m->mid->content,
		m->mid->next->content,
		(*m->end)->content,
		(*m->beg)->content);
}

void	dprint_tail_info(struct s_merge *m)
{
	if (m->tail == NULL || *m->tail == NULL || m->l == NULL || m->mid == NULL
		|| m->r == NULL || m->end == NULL || *m->end == NULL || m->beg == NULL)
		return (dvprint("Invalid pointer passed to dprint_tail_info\n"));
	if (NULL != (*m->end)->next && NULL != m->r->next)
		_print_one(m);
	else if (NULL == (*m->end)->next && NULL != m->r->next)
		_print_two(m);
	else if (NULL == (*m->end)->next)
		_print_three(m);
	else if (NULL == (*m->tail))
		_print_four(m);
	else
		dvprint("HUH?!\n");
}

void	print_diagnostics_input(t_list **lst, struct s_merge *m)
{
	dvprint("merge got l: \n");
	ft_lstprint_betw(*(m->beg), m->mid);
	dvprint("...and r: \n");
	ft_lstprint_betw(m->mid->next, *(m->end));
	dvprint("head: %s, nodecount:%d\n", (*lst)->content, m->nodec);
}

void	print_diagnostics_end(t_list **lst, struct s_merge *m)
{
	dvprint("DONE:\n");
	if (*m->tail && (*m->tail)->next)
		dprint("|beg->next...tail->next : %s->%s...%s->%s|\n",
			(*m->beg)->content, (*m->beg)->next->content, (*m->tail)->content,
			(*m->tail)->next->content);
	ft_lstprint_betw(*m->beg, *m->tail);
	ft_lstprint_betw(*m->beg, *m->end);
	ft_lstprint(*lst);
	dvprint("RETURNING head: (%p<-%s->%s). \n", (*lst)->prev, (*lst)->content,
		(*lst)->next->content);
}

void	print_prev_post(struct s_merge *m)
{
	if (NULL != (*m->tail)->prev && NULL != (*m->tail)->prev->prev)
		dvprint(MAGENTA " prev now linked %s<-%s->%s" RESET ") -> \n",
			(*m->tail)->prev->prev->content, (*m->tail)->prev->content,
			(*m->tail)->prev->next->content);
	else
		dvprint(MAGENTA " prev now linked %p<-%s->%s" RESET ") -> \n",
			(*m->tail)->prev->prev, (*m->tail)->prev->content,
			(*m->tail)->prev->next->content);
}

void	print_prev_pre(struct s_merge *m)
{
	if (NULL != (*m->tail)->prev)
		dvprint(BLUE " -reassign prev %s to:%s- " RESET,
			(*m->tail)->prev->content, m->tmp->content);
	else
		dvprint(BLUE " -reassign NULL prev to:%s- " RESET, m->tmp->content);
}

void	print_swap_post(t_list **lst, struct s_merge *m)
{
	if ((*m->tail) == *lst)
	{
		if ((*m->tail)->next)
			dvprint(CYAN "%s (head) "GREEN "(prev: %p, next: %s), -> \n" RESET,
				(*m->tail)->content, (*m->tail)->prev,
				(*m->tail)->next->content);
		else
			dvprint(CYAN "%s (head) "GREEN"(prev: %p, next: (?)), -> \n" RESET,
				(*m->tail)->content, (*m->tail)->prev);
	}
	else if ((*m->tail)->next)
		dvprint(CYAN "%s " GREEN "(prev: %s, next: %s), -> \n" RESET,
			(*m->tail)->content, (*m->tail)->prev->content,
			(*m->tail)->next->content);
	else
		dvprint(CYAN "%s " GREEN "(prev: %s, next: END), -> \n" RESET,
			(*m->tail)->content, (*m->tail)->prev->content);
}

void	print_tailnext_post(struct s_merge *m)
{
	if (*m->tail && m->tmp2)
		dvprint("rejoined to rest of list %s->%s\n", (*m->tail)->content,
			(m->tmp2)->content);
	if (*m->tail && (*m->end)->next && m->tmp2)
		dvprint(BLUE "status: end->next %s, tail:%s tail->next: %s,"
			"tmp2:%s\n" RESET,
			(*m->end)->next->content,
			(*m->tail)->content,
			(*m->tail)->next->content,
			(m->tmp2)->content);
	if (*m->tail)
		dvprint("%s (remainder, " MAGENTA "prev:%s->%s) -> " RESET,
			(*m->tail)->content, (*m->tail)->prev->content,
			(*m->tail)->prev->next->content);
}

void	print_tailadvance_post(struct s_merge *m)
{
	if (*m->tail && (*m->tail)->next && (*m->end)->content && m->tmp2)
		dvprint(BLUE "status: end->next %s, tail:%s tail->next: %s,"
			"tmp2: (%s)\n" RESET,
			(*m->end)->next->content,
			(*m->tail)->content,
			(*m->tail)->next->content,
			(m->tmp2)->content);
}

void	print_addremainder_post(struct s_merge *m)
{
	if ((*m->tail) && (*m->end) && ((*m->tail)->next && (*m->end)->next)
		&& m->tmp2)
		dvprint(BLUE "status:end->next %s, tail:%s tail->next: %s,"
			"tmp2:%s\n" RESET,
			(*m->end)->next->content,
			(*m->tail)->content,
			(*m->tail)->next->content,
			(m->tmp2)->content);
}
