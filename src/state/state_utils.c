#include "state_int.h"

/* Prints all env variables in state's t_env list */
void	s_env_print(t_state *s)
{
	if (!s)
		return ;
	return (env_print(s->sh_env_list));
}

/* Returns true if state's sh_env_list is non-NULL */
bool	has_sh_envp(t_state *s)
{
	if (!s)
		return (false);
	return (NULL != s->sh_env_list);
}
