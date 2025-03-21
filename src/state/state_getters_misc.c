#include "state_int.h"

char	*get_pwd(t_state *s)
{
	if (!s)
		return (NULL);
	return (s->pwd);
}

/* Returns the last exit code. */
int	*get_status(t_state *s)
{
	if (!s)
		return (NULL);
	return (&s->current_exit_code);
}
