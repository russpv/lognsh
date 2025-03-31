#ifndef ERROR_H
# define ERROR_H

# include "../globals/globals.h"
# include <errno.h>
# include <signal.h>
# include <stdio.h>
# include <unistd.h>
# include <stdbool.h>

# define EXIT_NULLCMD 0

# define ERR_AMBIGUOUS_REDIR 1
# define ERR_GENERAL 1
# define ERR_SYNTAX 2
# define ERR_FILE_NOT_FOUND 3
# define ERR_TOKEN 4
# define ERR_MEM 5
# define ERR_ARGS 1
# define ERR_INVALID_CMD_TYPE 7
# define ERR_FORK 8
# define ERR_INSUFFICIENT_CMDS 9
# define ERR_PIPE 10
# define ERR_ACCESS 11
# define ERR_DUP 12
# define ERR_CLOSE 13
# define ERR_WAITPID 14
# define ERR_GETCWD 15
# define ERR_CHDIR 16
# define ERR_REDIR 17
# define ERR_EXECVE 18
# define ERR_DUP2 19
# define ERR_BUFFLOW 20
# define ERR_STAT 21

# define ERR_CHILD_FAILED 127
# define ERR_CMD_NOT_FOUND 127
# define ERR_CMD_NOT_EXEC 126
# define ERR_CMD_SIGINTD 130
# define ERR_CHILD_SIGINT 130
# define ERR_EXIT_RANGE 255
# define ERR_CMD_IS_A_DIR 128

# define EMSG_MALLOC SHELL_NAME ": malloc"
# define EMSG_PIPE SHELL_NAME ": pipe"
# define EMSG_FORK SHELL_NAME ": fork"
# define EMSG_EXECVE SHELL_NAME ": execve"
# define EMSG_SIGACTION SHELL_NAME ": sigaction"
# define EMSG_ACCESS SHELL_NAME ": "
# define EMSG_OPEN SHELL_NAME ": open"
# define EMSG_CLOSE SHELL_NAME ": close"
# define EMSG_WAIT SHELL_NAME ": wait"
# define EMSG_WAITPID SHELL_NAME ": waitpid"
# define EMSG_GETCWD SHELL_NAME ": getcwd"
# define EMSG_CHDIR SHELL_NAME ": chdir"
# define EMSG_STAT SHELL_NAME ": stat"
# define EMSG_LSTAT SHELL_NAME ": lstat"
# define EMSG_FSTAT SHELL_NAME ": fstat"
# define EMSG_DUP SHELL_NAME ": dup"
# define EMSG_DUP2 SHELL_NAME ": dup2"
# define EMSG_IOCTL SHELL_NAME ": ioctl"
# define EMSG_TCSETATTR SHELL_NAME ": tcsetattr"
# define EMSG_TCGETATTR SHELL_NAME ": tcgetattr"

# define LOGMSG_SIGINT "interrupted\n"

// no includes, but redefs, due to circular dependency
typedef struct s_global_state	t_state;
struct s_lex;
typedef struct s_lex	t_lex;

/* specific error messages */
void							print_command_not_found(const char *cmd);
void							print_perror(const char *dingus);
void							print_lex_buffer_overflow(void);
void							print_ambiguous_redirect(const char *orig_fn);
void							print_too_many_args(const char *caller);
void							print_redirect_error(char *topath);
void							print_custom_err(const char *dingus,
									const char *msg);
void							print_custom_err_err(const char *dingus,
									const char *gizmo, const char *msg);
void							print_parse_error(t_state *s, const char *word,
									size_t pos);
void							print_lex_error(t_lex *l, char *word);

void							print_bufflow(void);
void							print_is_dir(void);

bool							is_error(int v);

/* To avoid circular includes: */
extern void						set_error(t_state *s, int e);
extern size_t					ft_strlen(const char *s);
void							perror(const char *);

#endif
