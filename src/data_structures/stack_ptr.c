#include "stack_int.h"

t_ptr_stack	*st_ptr_create(void)
{
	t_ptr_stack	*s;

	s = malloc(sizeof(struct s_ptr_stack));
	if (s)
	{
		ft_memset(&s->st, 0, MAX_ST_DEPTH * sizeof(void*));
		s->depth = 0;
	}
	return (s);
}
void	st_ptr_destroy(t_ptr_stack *s)
{
	free(s);
}

int		st_ptr_push(t_ptr_stack *s, void *thing)
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

void	*st_ptr_pop(t_ptr_stack *s)
{
	void	*tmp;

	if (s->depth == 0)
	{
		debug_print("Stack underflow!\n");
		return (NULL);
	}
	tmp = s->st[s->depth - 1];
	s->st[s->depth - 1] = 0;
	s->depth--;
	return (tmp);
}
/* Returns top ptr on stack or -1 if empty */
void	*st_ptr_peek(t_ptr_stack *s)
{
	void	*thing;

	if (s->depth == 0)
		return (NULL);
	thing = s->st[s->depth - 1];
	return (thing);
}
