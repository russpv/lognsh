#include "llist_int.h"
#include <stdio.h>

void debug_print_tail_info(struct s_merge *m) 
{
    // Ensure pointers are valid before accessing them
    if (m->tail == NULL || *m->tail == NULL || m->l == NULL || m->mid == NULL || m->r == NULL || m->end == NULL || *m->end == NULL || m->beg == NULL) 
	{
        debug_print("Invalid pointer passed to debug_print_tail_info\n");
        return;
    }

    // Handle different cases based on the presence of next pointers
    if (NULL != (*m->end)->next && NULL != m->r->next) {
        debug_print(BLUE "(tail now at: %s | l at: %s, l->next: %s, mid: %s, mid->next: %s "
                      "| r at: %s, r->next: %s, end: %s, end->next: %s "
                      "| beg: %s) \n" RESET,
                    (*m->tail)->content, m->l->content, m->l->next->content, m->mid->content, m->mid->next->content,
                    m->r->content, m->r->next->content, (*m->end)->content, (*m->end)->next->content,
                    (*m->beg)->content);
    }
    else if (NULL == (*m->end)->next && NULL != m->r->next) {
        debug_print(BLUE "(tail now at: %s | l at: %s, l->next: %s, mid: %s, mid->next: %s "
                      "| r at: %s, r->next: %s, end: %s, end->next: (end)) "
                      "| beg: %s) \n" RESET,
                    (*m->tail)->content, m->l->content, m->l->next->content, m->mid->content, m->mid->next->content,
                    m->r->content, m->r->next->content, (*m->end)->content,
                    (*m->beg)->content);
    }
    else if (NULL == (*m->end)->next) {
        debug_print(BLUE "(tail now at: %s | l at: %s, l->next: %s, mid: %s, mid->next: %s "
                      "| r at: (end), r->next: (end), end: %s, end->next: (end)) "
                      "| beg: %s) \n" RESET,
                    (*m->tail)->content, m->l->content, m->l->next->content, m->mid->content, m->mid->next->content,
                    (*m->end)->content,
                    (*m->beg)->content);
    }
    else if (NULL == (*m->tail)) {
        debug_print(BLUE "(tail now at: end | l at: %s, l->next: %s, mid: %s, mid->next: %s "
                      "| r at: (end), r->next: (end), end: %s, end->next: (end)) "
                      "| beg: %s) \n" RESET,
                    m->l->content, m->l->next->content, m->mid->content, m->mid->next->content,
                    (*m->end)->content,
                    (*m->beg)->content);
    }
    else {
        debug_print("HUH?!\n");
    }
}

void	print_diagnostics_input(t_list **lst, struct s_merge *m)
{
	debug_print( "merge got l: \n");
	ft_lstprint_betw(*(m->beg), m->mid);
	debug_print( "...and r: \n");
	ft_lstprint_betw(m->mid->next, *(m->end));
	debug_print( "head: %s, nodecount:%d\n", (*lst)->content, m->nodec);
}
void	print_diagnostics_end(t_list **lst, struct s_merge *m)
{
	debug_print( "DONE:\n");
	if (*m->tail && (*m->tail)->next)
		debug_print( "|beg->next...tail->next : %s->%s...%s->%s|\n", (*m->beg)->content, (*m->beg)->next->content, (*m->tail)->content, (*m->tail)->next->content);
	ft_lstprint_betw(*m->beg, *m->tail);
	ft_lstprint_betw(*m->beg, *m->end);
	ft_lstprint(*lst);
	debug_print( "RETURNING head: (%p<-%s->%s). \n", (*lst)->prev, (*lst)->content, (*lst)->next->content);
}

void	print_prev_post(struct s_merge *m)
{
	if (NULL != (*m->tail)->prev && NULL != (*m->tail)->prev->prev)
		debug_print( MAGENTA" prev now linked %s<-%s->%s"RESET") -> \n", \
			(*m->tail)->prev->prev->content, (*m->tail)->prev->content, (*m->tail)->prev->next->content);
	else 
		debug_print( MAGENTA" prev now linked %p<-%s->%s"RESET") -> \n", \
			(*m->tail)->prev->prev, (*m->tail)->prev->content, (*m->tail)->prev->next->content);
}

void	print_prev_pre(struct s_merge *m)
{
	if (NULL != (*m->tail)->prev)
	debug_print(BLUE" -reassign prev %s to:%s- "RESET, (*m->tail)->prev->content, m->tmp->content);
	else
	debug_print(BLUE" -reassign NULL prev to:%s- "RESET, m->tmp->content);
}

void	print_swap_post(t_list **lst, struct s_merge *m)
{
	if ((*m->tail) == *lst){
		debug_print( CYAN"%s (head) "GREEN"(prev: %p, next: %s), -> \n"RESET, (*m->tail)->content, (*m->tail)->prev, (*m->tail)->next->content);
	}
	else if ((*m->tail)->next)
		debug_print( CYAN"%s "GREEN"(prev: %s, next: %s), -> \n"RESET, (*m->tail)->content, (*m->tail)->prev->content, (*m->tail)->next->content);
	else
		debug_print( CYAN"%s "GREEN"(prev: %s, next: END), -> \n"RESET, (*m->tail)->content, (*m->tail)->prev->content);
}

void	print_tailnext_post(struct s_merge *m)
{
	if (*m->tail && m->tmp2)
		debug_print( "rejoined to rest of list %s->%s\n", (*m->tail)->content, (m->tmp2)->content);
	if (*m->tail && (*m->end)->next && m->tmp2)
		debug_print( BLUE"status: end->next %s, tail:%s tail->next: %s, tmp2:%s\n"RESET, (*m->end)->next->content, (*m->tail)->content,\
		(*m->tail)->next->content, (m->tmp2)->content);
	if (*m->tail)
		debug_print( "%s (remainder, "MAGENTA"prev:%s->%s) -> "RESET, (*m->tail)->content, (*m->tail)->prev->content, (*m->tail)->prev->next->content);
}

void	print_tailadvance_post(struct s_merge *m)
{
	if (*m->tail && (*m->tail)->next && (*m->end)->content && m->tmp2)
		debug_print( BLUE"status: end->next %s, tail:%s tail->next: %s, tmp2: (%s)\n"RESET, (*m->end)->next->content, (*m->tail)->content,\
		(*m->tail)->next->content, (m->tmp2)->content);
}

void	print_addremainder_post(struct s_merge *m)
{
	if ((*m->tail) && (*m->end) && ((*m->tail)->next && (*m->end)->next) && m->tmp2)
		debug_print( BLUE"status:end->next %s, tail:%s tail->next: %s, tmp2:%s\n"RESET, (*m->end)->next->content, (*m->tail)->content,\
		(*m->tail)->next->content, (m->tmp2)->content);
}

/*
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
*/