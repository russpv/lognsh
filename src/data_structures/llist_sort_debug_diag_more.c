/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   llist_sort_debug_diag_more.c                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rpeavey <rpeavey@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/10 16:30:36 by rpeavey           #+#    #+#             */
/*   Updated: 2025/04/10 16:30:37 by rpeavey          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "llist_int.h"

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
