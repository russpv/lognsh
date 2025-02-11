#include "stack_int.h"

t_stack	*st_create(void)
{
	t_stack	*s;

	s = malloc(sizeof(struct s_stack));
	if (s)
	{
		ft_memset(&s->st, 0, MAX_ST_DEPTH * sizeof(int));
		s->depth = 0;
	}
	return (s);
}
void	st_destroy(t_stack *s)
{
	free(s);
}

int	st_push(t_stack *s, int thing)
{
	if (s->depth >= MAX_ST_DEPTH)
	{
		debug_print("Stack overflow!\n");
		return (1);
	}
	s->st[s->depth] = thing;
	s->depth++;
	return (0);
}

int	st_pop(t_stack *s)
{
	int	tmp;

	if (s->depth == 0)
	{
		debug_print("Stack underflow!\n");
		return (-1);
	}
	tmp = s->st[s->depth - 1];
	s->st[s->depth - 1] = 0;
	s->depth--;
	return (tmp);
}

/* Returns top number on stack or -1 if empty */
int	st_peek(t_stack *s)
{
	int	thing;

	if (s->depth == 0)
		return (-1);
	thing = s->st[s->depth - 1];
	return (thing);
}
