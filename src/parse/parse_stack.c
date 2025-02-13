#include "parse_int.h"

#define MAX_STACK_DEPTH 100

t_pstack	*create_stack(void)
{
	t_pstack	*s;

	s = malloc(sizeof(t_pstack));
	if (s)
		s->depth = 0;
	return (s);
}

void	destroy_stack(t_pstack *s)
{
	free(s);
}

int	push(t_pstack *stack)
{
	if (stack->depth < MAX_STACK_DEPTH)
	{
		stack->depth++;
		return (0);
	}
	else
	{
		debug_print("Parser: ERROR: Exceeded maximum parentheses.\n");
		return (1);
	}
}

int	pop(t_pstack *stack)
{
	if (--stack->depth < 0)
	{
		debug_print("Parser: ERROR: Unmatched parentheses.\n");
		return (1);
	}
	return (0);
}
