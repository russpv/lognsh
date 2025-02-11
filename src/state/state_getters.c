#include "state_int.h"


char *get_input(t_state *s)
{
	return (s->input);
}

/* Returns new heap array each call */
char	**get_envp(t_state *s)
{
	extern char **environ;

	if (!s)
		return (NULL);
	if (!s->sh_env_list)
		s->sh_env_list = copy_envp(environ);
	return (lst_to_array(s->sh_env_list));
}

t_cmd	*get_cmd(t_state *s)
{
	return (s->current_cmd);
}

int	*get_status(t_state *s)
{
	return(&s->current_exit_code);
}

/* Passthrough getter to retrieve individual v in k:v */
t_env	*get_sh_env(t_state *s, const char *key)
{
	if (!s)
		return (NULL);
	if (!s->sh_env_list)
		return (NULL);
	return (env_getenv_value(key, s->sh_env_list));
}
