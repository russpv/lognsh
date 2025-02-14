#ifndef ERROR_H
# define ERROR_H

# include "../globals/globals.h"
#include <signal.h>
#include <unistd.h>

# define ERR_NONE 0
# define ERR_AMBIGUOUS_REDIR 1
# define ERR_GENERAL 1
# define ERR_SYNTAX 2
# define ERR_FILE_NOT_FOUND 3
# define ERR_TOKEN 4
# define ERR_MEM 5
# define ERR_ARGS 6
# define ERR_INVALID_CMD_TYPE 7
# define ERR_FORK 8
# define ERR_INSUFFICIENT_CMDS 9
# define ERR_PIPE 10

# define ERR_CMD_NOT_FOUND 127
# define ERR_CMD_NOT_EXEC 126
# define ERR_CMD_SIGINTD 130
# define ERR_EXIT_RANGE 255

#define ERRMSG_MALLOC "ERR malloc\n"
#define ERRMSG_PIPE "ERR pipe()\n"
#define ERRMSG_FORK "ERR fork()\n"
#define ERRMSG_EXECVE "ERR execve()\n"

// no includes, but redefs, due to circular dependency
typedef struct s_global_state	t_state;

void							*null_and_stat(t_state *s, int error_code);
const char						*get_error_message(int error_code);

/* specific error messages */
void							print_command_not_found(const char *cmd);
void							print_permission_denied(const char *path);
void							print_lex_buffer_overflow(void);
void							print_ambiguous_redirect(const char *orig_fn);
void							print_too_many_args(const char *caller);
void	print_redirect_error(char *topath);

/* To avoid circular includes: */
extern void	set_error(t_state *s, int e);
extern size_t	ft_strlen(const char *s);

#endif
