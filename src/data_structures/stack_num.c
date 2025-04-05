/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   stack_num.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dayeo <dayeo@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/05 09:32:40 by dayeo             #+#    #+#             */
/*   Updated: 2025/04/05 09:32:42 by dayeo            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "stack_int.h"

#define DBGMSG_STPUSH_OVERFLOW "ERROR Stack overflow!\n"
#define DBGMSG_STPOP_UNDERFL "ERROR Stack underflow!\n"

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

int	st_int_push(t_int_stack *s, int thing)
{
	if (s->depth >= MAX_ST_DEPTH)
	{
		debug_print(DBGMSG_STPUSH_OVERFLOW);
		return (1);
	}
	s->st[s->depth] = thing;
	s->depth++;
	return (0);
}

int	st_int_pop(t_int_stack *s)
{
	int	tmp;

	if (s->depth == 0)
	{
		debug_print(DBGMSG_STPOP_UNDERFL);
		return (-1);
	}
	tmp = s->st[s->depth - 1];
	s->st[s->depth - 1] = 0;
	s->depth--;
	return (tmp);
}

/* Returns top number on stack or -1 if empty */
int	st_int_peek(t_int_stack *s)
{
	int	thing;

	if (s->depth == 0)
		return (-1);
	thing = s->st[s->depth - 1];
	return (thing);
}
