#ifndef STATE_H
# define STATE_H

# include "env.h"
# include "../signal/signal.h"
# include "../command/command.h"


/* STATE ADT
** A centralized singleton-like pattern for tracking global state
** including environment, command history, execution state.
**
*/

typedef struct s_state *t_state;


// TODO move this into sub header for ADT
struct s_global_state {
	char **path; //PATH is char **
	char *pwd; //cwd modifies this, pwd prints this, execution starts with searching here
	int current_exit_code; //last command's exit code
	// TODO track stdin, stdout, stderr fds to undo redirection
	// TODO add command history pointer
	// TODO 
	// no signals, too mcuh indirection	
	// t_ht *env_cache; TODO LATER, add hash table to env.h for faster lookups
};

// Methods
t_state	*init_state(void);
void	destroy_state(t_state *state);

int	set_exit_status(t_state *state);

t_cmd	build_command(char *input); //interface to COMMAND

char	*get_env_var(t_state *s, const char *key);
void	set_env_var(t_state *s, const char *key, const char *value);
char	*search_path(const char *command); //wrapper required for COMMAND
char	**expand_glob(const char *pattern); //wrapper required for COMMAND

#endif

