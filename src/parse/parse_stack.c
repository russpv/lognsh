#include "parse_int.h"

#define MAX_STACK_DEPTH 100

t_stack *create_stack(void)
{
	t_stack *s;

	s = malloc(sizeof(t_stack));
	if (s)
		s->depth = 0;
	return (s);
}

void	destroy_stack(t_stack *s)
{
	free(s);
}

void	push(t_stack *stack)
{
	if (stack->depth < MAX_STACK_DEPTH)
		stack->depth++;
	else
		debug_print("ERROR: Exceeded maximum parentheses.\n");
}

int	pop(t_stack *stack)
{
	if (--stack->depth < 0)
	{
		debug_print("ERROR: Unmatched parentheses.\n");
		return (1);
	}
	return (0);
}