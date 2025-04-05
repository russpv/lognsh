#include "llist_int.h"
#include <stdio.h>

static void _print_one(struct s_merge *m)
{
	debug_print(BLUE "(tail now at: %s | l at: %s, l->next: %s, mid: %s,"
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

static void _print_two(struct s_merge *m)
{
	debug_print(BLUE "(tail now at: %s | l at: %s, l->next: %s, mid: %s,"
		"mid->next: %s | r at: %s, r->next: %s, end: %s, end->next: (end)) "
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

static void _print_three(struct s_merge *m)
{
	debug_print(BLUE "(tail now at: %s | l at: %s, l->next: %s, mid: %s,"
		"mid->next: %s | r at: (end), r->next: (end), end: %s, end->next: (end)) "
		"| beg: %s) \n" RESET,
		(*m->tail)->content,
		m->l->content,
		m->l->next->content,
		m->mid->content,
		m->mid->next->content,
		(*m->end)->content,
		(*m->beg)->content);
}

static void _print_four(struct s_merge *m)
{
	debug_print(BLUE "(tail now at: end | l at: %s, l->next: %s, mid: %s,"
		"mid->next: %s | r at: (end), r->next: (end), end: %s, end->next: (end)) "
		"| beg: %s) \n" RESET,
		m->l->content,
		m->l->next->content,
		m->mid->content,
		m->mid->next->content,
		(*m->end)->content,
		(*m->beg)->content);
}

void	debug_print_tail_info(struct s_merge *m)
{
	if (m->tail == NULL || *m->tail == NULL || m->l == NULL || m->mid == NULL
		|| m->r == NULL || m->end == NULL || *m->end == NULL || m->beg == NULL)
		return(debug_print("Invalid pointer passed to debug_print_tail_info\n"));
	if (NULL != (*m->end)->next && NULL != m->r->next)
		_print_one(m);
	else if (NULL == (*m->end)->next && NULL != m->r->next)
		_print_two(m);
	else if (NULL == (*m->end)->next)
		_print_three(m);
	else if (NULL == (*m->tail))
		_print_four(m);
	else
		debug_print("HUH?!\n");
}
