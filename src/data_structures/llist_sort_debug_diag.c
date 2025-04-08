#include "llist_int.h"
#include <stdio.h>

void	print_diagnostics_input(t_list **lst, struct s_merge *m)
{
	debug_print("merge got l: \n");
	ft_lstprint_betw(*(m->beg), m->mid);
	debug_print("...and r: \n");
	ft_lstprint_betw(m->mid->next, *(m->end));
	debug_print("head: %s, nodecount:%d\n", (*lst)->content, m->nodec);
}

void	print_diagnostics_end(t_list **lst, struct s_merge *m)
{
	debug_print("DONE:\n");
	if (*m->tail && (*m->tail)->next)
		debug_print("|beg->next...tail->next : %s->%s...%s->%s|\n",
			(*m->beg)->content, (*m->beg)->next->content, (*m->tail)->content,
			(*m->tail)->next->content);
	ft_lstprint_betw(*m->beg, *m->tail);
	ft_lstprint_betw(*m->beg, *m->end);
	ft_lstprint(*lst);
	debug_print("RETURNING head: (%p<-%s->%s). \n", (*lst)->prev,
		(*lst)->content, (*lst)->next->content);
}

void	print_prev_post(struct s_merge *m)
{
	if (NULL != (*m->tail)->prev && NULL != (*m->tail)->prev->prev)
		debug_print(MAGENTA " prev now linked %s<-%s->%s" RESET ") -> \n",
			(*m->tail)->prev->prev->content, (*m->tail)->prev->content,
			(*m->tail)->prev->next->content);
	else
		debug_print(MAGENTA " prev now linked %p<-%s->%s" RESET ") -> \n",
			(*m->tail)->prev->prev, (*m->tail)->prev->content,
			(*m->tail)->prev->next->content);
}

void	print_prev_pre(struct s_merge *m)
{
	if (NULL != (*m->tail)->prev)
		debug_print(BLUE " -reassign prev %s to:%s- " RESET,
			(*m->tail)->prev->content, m->tmp->content);
	else
		debug_print(BLUE " -reassign NULL prev to:%s- " RESET, m->tmp->content);
}

void	print_swap_post(t_list **lst, struct s_merge *m)
{
	if ((*m->tail) == *lst)
	{
		if ((*m->tail)->next)
			debug_print(CYAN "%s (head) " GREEN "(prev: %p, next: %s), -> \n" RESET,
				(*m->tail)->content, (*m->tail)->prev, (*m->tail)->next->content);
		else
			debug_print(CYAN "%s (head) " GREEN "(prev: %p, next: (?)), -> \n" RESET, //%s
				(*m->tail)->content, (*m->tail)->prev); // , (*m->tail)->next->content);
	}
	else if ((*m->tail)->next)
		debug_print(CYAN "%s " GREEN "(prev: %s, next: %s), -> \n" RESET,
			(*m->tail)->content, (*m->tail)->prev->content,
			(*m->tail)->next->content);
	else
		debug_print(CYAN "%s " GREEN "(prev: %s, next: END), -> \n" RESET,
			(*m->tail)->content, (*m->tail)->prev->content);
}
