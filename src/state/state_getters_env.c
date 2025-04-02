#include "state_int.h"

/* Returns new heap array each call */
char	**get_envp(t_state *s)
{
	extern char	**environ;

	if (!s)
		return (NULL);
	if (!s->sh_env_list)
		s->sh_env_list = copy_envp(get_mem(s), environ);
	return (env_to_array(get_mem(s), s->sh_env_list));
}

/* Passthrough getter to retrieve individual v in k:v */
char	*get_env_val(t_state *s, const char *key)
{
	if (!s)
		return (NULL);
	if (!s->sh_env_list)
		return (NULL);
	return (env_find_value(key, s->sh_env_list));
}

t_env	*get_env_list(t_state *s)
{
	if (!s)
		return (NULL);
	return (s->sh_env_list);
}

t_env	**get_env_list_ptr(t_state *s)
{
	if (!s)
		return (NULL);
	return (&s->sh_env_list);
}
