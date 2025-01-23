#ifndef STATE_H
# define STATE_H

# include "../command/command.h"
# include "../signal/signal.h"
# include "env.h"
# include "error.h"
# include <readline/history.h>
# include <readline/readline.h>

/* Bash Manual
 *
 * For the shell’s purposes,
 * a command which exits with a zero exit status has succeeded. 
 * A non-zero exit status indicates failure. This seemingly counter-intuitive scheme is used 
 * so there is one well-defined way to indicate success and a variety of ways to indicate 
 * various failure modes. When a command terminates on a fatal signal whose number is N,
 * Bash uses the value 128+N as the exit status.
 * 
 * If a command is not found, the child process created to execute it returns a status of 127. 
 * If a command is found but is not executable, the return status is 126.
 * 
 * If a command fails because of an error during expansion or redirection, the exit status is greater than zero.
 * 
 * 
 */


/* STATE ADT
** A centralized singleton-like pattern for tracking global state
** including environment, command history, execution state.
**
*/

typedef struct s_state	*t_state;

// TODO move this into sub header for ADT
struct					s_global_state
{
	char **path;           // PATH is char **
	char *pwd;             // cwd modifies this, pwd prints this,
		execution starts with searching here
	int current_exit_code; // last command's exit code
	// TODO track stdin, stdout, stderr fds to undo redirection
	// TODO add command history pointer
	// TODO
	// no signals, too mcuh indirection
	// t_ht *env_cache; TODO LATER, add hash table to env.h for faster lookups
};

// Methods
t_state					*init_state(void);
void					destroy_state(t_state *state);

int						set_exit_status(t_state *state);

t_cmd	build_command(char *input); // interface to COMMAND

char					*get_env_var(t_state *s, const char *key);
void					set_env_var(t_state *s, const char *key,
							const char *value);
char	*search_path(const char *command);  // wrapper required for COMMAND
char	**expand_glob(const char *pattern); // wrapper required for COMMAND

#endif
