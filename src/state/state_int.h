#include "state.h"

struct s_global_state {
	char **path; //PATH is char **
	char *pwd; //cwd modifies this, pwd prints this, execution starts with searching here
	int current_exit_code; //last command's exit code
	// TODO track stdin, stdout, stderr fds to undo redirection
	// TODO add command history pointer
	// TODO 
	// no signals, too mcuh indirection	
	// t_ht *env_cache; TODO LATER, add hash table to env.h for faster lookups
}

t_state	*init_state(void)
{
 	//TODO
}

void	destroy_state(t_state *state)
{
	//TODO
}

int	set_exit_status(t_state *state)
{
	//TODO
}
