#ifndef STATE_H
# define STATE_H

# include "../debug.h"
# include "../../include/libft.h"
# include "../signal/signal.h"
# include "error.h"
# include <readline/history.h>
# include <readline/readline.h>
# include <stdbool.h>

# define SHELL_NAME "minish"
# define MAX_RAW_INPUT_LEN 2056

# ifdef TESTMODE
#  define TESTFLAG 1
# else
#  define TESTFLAG 0
# endif

/* Bash Manual
 *
 * For the shell’s purposes,
 * a command which exits with a zero exit status has succeeded.

	* A non-zero exit status indicates failure. This seemingly
		counter-intuitive scheme is used
	* so there is one well-defined way to indicate success and
	a variety of ways to indicate various failure modes. When
	a command terminates on a fatal signal whose number is N,
 * Bash uses the value 128+N as the exit status.

 * If a command is not found,
	the child process created to execute it returns a status of 127.
 * If a command is found but is not executable, the return
	status is 126.
 *
 * If a command fails because of an error during expansion or redirection,
	the exit status is greater than zero.
 */

/* STATE ADT
** A centralized singleton-like pattern for tracking global state
** including environment, command history, execution state.
*/
struct							s_global_state;
typedef struct s_global_state	t_state;

typedef void					(*t_destroy_fn)(void *instance);
typedef void					(*t_attach_fn)(void *instance);

/* Duplicate declarations instead of includes */
typedef struct s_parser			t_parser;
typedef struct s_lex			t_lex;
typedef struct s_cmd			t_cmd;
typedef struct s_env			t_env;

/* Methods */
t_state							*init_state(char **envp);
void							destroy_state(t_state *state);

int								set_exit_status(t_state *state, int value);
void							set_error(t_state *state, int code);
void							set_parser(t_state *state, t_parser *p);
void							set_lexer(t_state *state, t_lex *l);
void							set_input(t_state *s, char *input);
void							set_command(t_state *s, t_cmd *c);

int								*get_status(t_state *s);
char							*get_input(t_state *s);
t_cmd							*get_cmd(t_state *s);

char							**get_envp(t_state *s);
char							*get_sh_env(t_state *s, const char *key);
char							**get_sh_path(t_state *s);
t_env							*get_sh_env_list(t_state *s);
t_env							**get_sh_env_list_add(t_state *s);
char							*get_pwd(t_state *s);
// int	set_sh_env(t_state *s, const char *key, const char *value);
		//For export()

void							register_command_destroy(t_state *s,
									t_destroy_fn fn);
void							register_parser_destroy(t_state *s,
									t_destroy_fn fn);
void							register_lexer_destroy(t_state *s,
									t_destroy_fn fn);
void							s_free_cmd(t_state *s);
void							s_free_cmd_lex_parse(t_state *state);

bool							has_sh_envp(t_state *s);

#endif
