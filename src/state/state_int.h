#include "state.h"

struct s_global_state {
	char **path;
	char *pwd; 
	int current_exit_code; 
	int error_code;
};
typedef struct s_global_state	t_state;

	// TODO track stdin, stdout, stderr fds to undo redirection
	// TODO add command history pointer
	// TODO 
	// no signals, too mcuh indirection	
	// t_ht *env_cache; TODO LATER, add hash table to env.h for faster lookups

t_state	*init_state(void)
{
 	t_state *s = malloc(sizeof(struct s_global_state));
	if (s)
	{
		s->path = NULL;
		s->pwd = NULL;
		s->current_exit_code = 0;
		s->error_code = 0;
	}
	return (s);
}

void	destroy_state(t_state *state)
{
	//TODO
	(void)state;
}



