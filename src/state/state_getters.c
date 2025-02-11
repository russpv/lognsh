#include "state_int.h"


char *get_input(t_state *s)
{
	return (s->input);
}

char	**get_envp(t_state *s)
{
	if (!s)
		return (NULL);
	if (!s->env_list)
		return (NULL);
	return (lst_to_array(s->env_list));
}

t_cmd	*get_cmd(t_state *s)
{
	return (s->current_cmd);
}

int	*get_status(t_state *s)
{
	return(&s->current_exit_code);
}

