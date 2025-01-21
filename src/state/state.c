#include "state_int.h"

int	set_exit_status(t_state *state)
{
	//TODO
}

/* BUILD COMMAND
 * Use command.h methods
 */
t_cmd	build_command(char *input)
{
	//TODO, use command.h methods
}

char	*get_env_var(t_state *s, const char *key)
{
	//TODO, use env.h methods
}

void	set_env_var(t_state *s, const char *key, const char *value)
{
	//TODO, use env.h methods
}

char	*search_path(const char *command)
{
	//TODO
	//wrapper required for COMMAND for parsing
}

char	**expand_glob(const char *pattern)
{
	//TODO
	//wrapper required for COMMAND for parsing
}


