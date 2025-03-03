#include "state_int.h"

/* Returns true if state's sh_env_list is non-NULL */
bool	has_sh_envp(t_state *s)
{
	if (!s)
		return (false);
	return (NULL != s->sh_env_list);
}
