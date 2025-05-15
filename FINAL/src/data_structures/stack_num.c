/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   stack_num.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rpeavey <rpeavey@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/10 16:31:22 by rpeavey           #+#    #+#             */
/*   Updated: 2025/04/12 19:46:08 by rpeavey          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "stack_int.h"

t_int_stack	*st_int_copy(t_mem_mgr *m, t_int_stack *ref)
{
	t_int_stack	*s;
	int			i;

	i = -1;
	s = m->f(&m->list, sizeof(struct s_int_stack));
	if (s)
	{
		while (++i < MAX_ST_DEPTH)
			s->st[i] = ref->st[i];
		s->depth = ref->depth;
	}
	return (s);
}

t_int_stack	*st_int_create(t_mem_mgr *m)
{
	t_int_stack	*s;

	s = m->f(&m->list, sizeof(struct s_int_stack));
	if (s)
	{
		ft_memset(&s->st, 0, MAX_ST_DEPTH * sizeof(int));
		s->depth = 0;
	}
	return (s);
}

void	st_int_destroy(t_mem_mgr *m, t_int_stack *s)
{
	m->dealloc(&m->list, s);
}
