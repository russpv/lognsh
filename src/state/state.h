#ifndef STATE_H
# define STATE_H

# include "../debug.h"
# include "../signal/signal.h"
# include "env.h"
# include "error.h"
# include <errno.h>
# include <readline/readline.h>
# include <readline/history.h>

# define SHELL_NAME "minish"

# ifdef TESTMODE
#  define TESTFLAG 1
# else
#  define TESTFLAG 0
# endif

/* Bash Manual
 *
 * For the shell’s purposes,
 * a command which exits with a zero exit status has succeeded.

	* A non-zero exit status indicates failure. This seemingly counter-intuitive scheme is used

	* so there is one well-defined way to indicate success and a variety of ways to indicate

	* various failure modes. When a command terminates on a fatal signal whose number is N,
 * Bash uses the value 128+N as the exit status.
 *
 * If a command is not found,
	the child process created to execute it returns a status of 127.
 * If a command is found but is not executable, the return status is 126.
 *
 * If a command fails because of an error during expansion or redirection,
	the exit status is greater than zero.
 *
 *
 */

/* STATE ADT
** A centralized singleton-like pattern for tracking global state
** including environment, command history, execution state.
*/
struct s_global_state;
typedef struct s_global_state	t_state;

typedef void					(*t_destroy_fn)(void *instance);
typedef void					(*t_attach_fn)(void *instance);

/* Duplicate declarations instead of includes */
typedef struct s_parser			t_parser;
typedef struct s_lex			t_lex;
typedef struct s_cmd			t_cmd;

// Methods
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

void							register_command_destroy(t_state *s,
									t_destroy_fn fn);
void							register_parser_destroy(t_state *s,
									t_destroy_fn fn);
void							register_lexer_destroy(t_state *s,
									t_destroy_fn fn);
void							s_free_cmd(t_state *s);

extern char						*readline(const char *prompt);
extern void						add_history(const char *line);
extern void						rl_replace_line(const char *text, \
									int clear_undo);
extern void						rl_redisplay(void);
extern int						rl_on_new_line(void);

#endif
