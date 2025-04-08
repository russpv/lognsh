#include "llist_int.h"

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
