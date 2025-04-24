
#include "state_int.h"

// Returns memory manager struct
t_mem_mgr	*get_mem(t_state *s)
{
	if (!s)
		return (NULL);
	return (&s->mem_mgr);
}

int	get_tmp_flag(t_state *s)
{
	if (!s)
		return (INT_MAX);
	return (s->tmp_flag);
}

char	*get_tmp(t_state *s)
{
	if (!s)
		return (NULL);
	return (s->tmp);
}
