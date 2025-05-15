/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   stack_int_acts.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rpeavey <rpeavey@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/12 19:45:42 by rpeavey           #+#    #+#             */
/*   Updated: 2025/04/12 19:46:03 by rpeavey          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "stack_int.h"

int	st_int_push(t_int_stack *s, int thing)
{
	if (s->depth >= MAX_ST_DEPTH)
	{
		dprint(DBGMSG_STPUSH_OVERFLOW);
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
		dprint(DBGMSG_STPOP_UNDERFL);
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
