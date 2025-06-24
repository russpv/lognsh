#include "parse_int.h"

t_pstack	*create_stack(t_mem_mgr *m)
{
	t_pstack	*s;

	s = m->f(&m->list, sizeof(t_pstack));
	if (!s)
		exit_clean(&m->list, ENOMEM, __FUNCTION__, EMSG_MALLOC);
	s->depth = 0;
	return (s);
}

void	destroy_stack(t_mem_mgr *m, t_pstack *s)
{
	m->dealloc(&m->list, s);
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
		dprint(_MOD_ ": ERROR: Exceeded maximum parentheses.\n");
		return (1);
	}
}

int	pop(t_pstack *stack)
{
	if (--stack->depth < 0)
	{
		dprint(_MOD_ ": ERROR: Unmatched parentheses.\n");
		return (1);
	}
	return (0);
}
