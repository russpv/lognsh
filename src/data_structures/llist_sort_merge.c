/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   llist_sort_merge.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dayeo <dayeo@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/05 09:31:47 by dayeo             #+#    #+#             */
/*   Updated: 2025/04/05 22:18:14 by dayeo            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "llist_int.h"

/* Adds the remaining nodes after one of the
 * left or right sub-lists is exhausted.
 */
static void	_add_remainder(struct s_merge *m)
{
	if (m->l != m->mid->next)
	{
		*m->tail = m->l;
		(*m->tail)->prev = m->tmp;
	}
	else if (m->r != m->tmp2)
	{
		*m->tail = m->r;
		(*m->tail)->prev = m->tmp;
	}
}

/* Swaps the current tail node for the correct
 * one across left and right sub-lists. Reassigns
 * prev ptr to where tail was last (tmp).
 */
static void	_do_swap(t_list **lst, struct s_merge *m)
{
	(void)lst;
	if (ALESSTHANB == compare(m->l, m->r))
	{
		*m->tail = m->l;
		m->l = m->l->next;
	}
	else if (AMORETHANB == compare(m->l, m->r))
	{
		*m->tail = m->r;
		m->r = m->r->next;
	}
	if ((*m->tail) != NULL && m->tmp != NULL)
		(*m->tail)->prev = m->tmp;
}

/* Advances to the end of the added remainder
 * and ensures next ptr is set to reconnect
 * with rest of list beyond this segment
 */
static void	_adv_and_reconnect(struct s_merge *m)
{
	while (++m->i < m->nodec)
		m->tail = &((*m->tail)->next);
	if (*m->tail)
		(*m->tail)->next = m->tmp2;
}

/* Ensures the true head and true tail are terminated NULL
 * in case any swap occurred.
 */
void	_terminate_ends(t_list **lst, struct s_merge *m)
{
	if ((*m->end)->next == NULL && *m->tail == *m->end)
		(*m->tail)->next = NULL;
	(*lst)->prev = NULL;
}

/* MERGESORT
 * Google it.
 * Note: Critical that the comparator function does not return ties or the tail
 * advances to whichever node->next.
 * Watch for:
 * - swapped node can be the head ptr with no prev!
 * - swapped node can be the end ptr with no next!
 * Critical that list segment boundary ptrs are updated for subsequent
 * recursive calls as stack unwinds, or segments may be skipped.
 */
void	merge(t_list **lst, t_list **beg, t_list *mid, t_list **end)
{
	struct s_merge	m;

	init_merge(&m, beg, end, mid);
	if (ALESSTHANB == compare(mid, mid->next))
		return (debug_print("already sorted.\n"));
	while ((NULL != m.l && NULL != m.r) && (mid->next != m.l
			&& (*end)->next != m.r) && (++m.i <= m.nodec))
	{
		assert(compare(m.l, m.r) != 0);
		_do_swap(lst, &m);
		m.tmp = *m.tail;
		m.tail = &((*m.tail)->next);
	}
	_add_remainder(&m);
	_adv_and_reconnect(&m);
	if (*m.tail)
		(*end) = (*m.tail);
	_terminate_ends(lst, &m);
	return ;
}
