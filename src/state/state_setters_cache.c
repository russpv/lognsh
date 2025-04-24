
#include "state_int.h"

void	set_tmp(t_state *s, char *str)
{
	if (NULL != s->tmp)
		(get_mem(s))->dealloc(&get_mem(s)->list, s->tmp);
	s->tmp = ft_strdup_tmp(get_mem(s), str);
}

void	set_tmp_flag(t_state *s, int val)
{
	if (NULL == s)
		return ;
	s->tmp_flag = val;
}
