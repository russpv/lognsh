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
