#include "state_int.h"

// print all env variables in state's t_env list
void	s_env_print(t_state *s)
{
	if (!s)
		return ;
	return (env_print(s->sh_env_list));
}
