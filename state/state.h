#ifndef STATE_H
# define STATE_H

# include "env.h"
# include "../signal/signal.h"

// A centralized singleton-like pattern for tracking global state

// Think of this as a 'hub' for the modules; the modules should not be directly
// dependent on each other, but rather interact here if at all
//

// implement facade for env.h for better decoupling and extensibility
// point to env elements and pass around in all/most? downstream modules
// import at high level in MINISHELL

// ADT forward 'definition'
typedef struct s_global_state *State;


// TODO move this into sub header for ADT
struct s_global_state {
	char **path; //PATH is char **
	char *pwd; //cwd modifies this, pwd prints this, execution starts with searching here
	int exit_code; //last command's exit code
	// TODO track stdin, stdout, stderr fds to undo redirection
	// TODO add command history pointer
	// TODO 
	// no signals, too mcuh indirection	
	// t_ht *env_cache; TODO LATER, add hash table to env.h for faster lookups
} State

// Methods
State	*create_global_state(void);
void	destroy_global_state(State *state);

void	set_exit_status(State *state);

#endif

