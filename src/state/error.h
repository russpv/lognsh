
#ifndef ERROR_H
# define ERROR_H

# include "../globals/globals.h"
# include <errno.h>
# include <signal.h>
# include <stdbool.h>
# include <stdio.h>
# include <unistd.h>
# include <sys/ioctl.h>

# define EXIT_NULLCMD 0
# define EX_HAPPY 0
# define EX_ERNDM 1
# define EX_EINVAL 2
# define EX_EREDIR 3

# define ERR_AMBIGUOUS_REDIR 1
# define ERR_GENERAL 1
# define ERR_SYNTAX 2
# define ERR_ARGS 3
# define ERR_TOKEN 4
# define ERR_DUP 5
# define ERR_FILE_NOT_FOUND 6
# define ERR_INVALID_CMD_TYPE 7
# define ERR_FORK 8
# define ERR_INSUFFICIENT_CMDS 9
# define ERR_PIPE 10
# define ERR_ACCESS 11
# define ERR_MEM 12
# define ERR_CLOSE 13
# define ERR_WAITPID 14
# define ERR_GETCWD 15
# define ERR_CHDIR 16
# define ERR_REDIR 17
# define ERR_EXECVE 18
# define ERR_DUP2 19
# define ERR_BUFFLOW 20
# define ERR_STAT 21

# define ERR_CMD_NOT_FOUND 127
# define ERR_CMD_NOT_EXEC 126
# define ERR_CHILD_SIGINT 130
# define ERR_EXIT_RANGE 255
# define ERR_CMD_IS_A_DIR 126

# define ERR_FN_ARG_REQD 2

# define ERR_RL_ABORTED -2
# define ERR_CMD_GOTSIG -1

# define EMSG_MALLOC "malloc"
# define EMSG_PIPE "pipe"
# define EMSG_FORK "fork"
# define EMSG_EXECVE "execve"
# define EMSG_SIGACTION "sigaction"
# define EMSG_ACCESS "access"
# define EMSG_OPEN "open"
# define EMSG_CLOSE "close"
# define EMSG_WAIT "wait"
# define EMSG_WAITPID "waitpid"
# define EMSG_GETCWD "getcwd"
# define EMSG_CHDIR "chdir"
# define EMSG_STAT "stat"
# define EMSG_LSTAT "lstat"
# define EMSG_FSTAT "fstat"
# define EMSG_DUP "dup"
# define EMSG_DUP2 "dup2"
# define EMSG_IOCTL "ioctl"
# define EMSG_TCSETATTR "tcsetattr"
# define EMSG_TCGETATTR "tcgetattr"

# define LOGMSG_SIGINT "interrupted\n"

// no includes, but redefs, due to circular dependency
typedef struct s_global_state	t_state;
struct							s_lex;
typedef struct s_lex			t_lex;

/* specific error messages */
void							print_command_not_found(const char *cmd);
void							print_perror(const char *dingus);
void							print_lex_buffer_overflow(void);
void							print_ambig_redir(const char *orig_fn);
void							print_too_many_args(const char *caller);
void							print_redirect_warning(char *topath);
void							print_filename_missing(const char *name);

void							print_custom_err(const char *dingus,
									const char *msg);
void							print_custom_err_err(const char *dingus,
									const char *gizmo, const char *msg);
void							print_parse_error(t_state *s, const char *word,
									size_t pos);
void							print_lex_error(t_lex *l, char *word);
void							print_hdoc_error(const char *line,
									const char *eof);
void							print_hdoc_eof_error(t_lex *l, char *word);
void							print_redir_error(t_state *s, const char *word,
									size_t pos);
void							print_nocmd_error(t_state *s, const char *word,
									size_t pos);
void							print_invalid_name(const char *caller,
									const char *name);
void							print_parse_redir_error(t_state *s, size_t pos);
void							print_dne(const char *path);
void							print_value_toolong(void);

void							pbufflow(const char *s);
void							print_is_dir(char *p);
void							print_inv_cmd(void);

bool							is_error(int v);
int								get_wincols(void);

/* To avoid circular includes*/
extern void						set_error(t_state *s, int e);
extern size_t					ft_strlen(const char *s);
extern char						*get_input(t_state *s);
extern char						*lex_get_ptr(t_lex *l);
extern const char				*lex_get_raw(t_lex *l);
extern void						ft_putstr_fd(char const *s, int fd);
extern char						*ft_strchr(const char *s, int c_in);
extern size_t					ft_strnlen(const char *s, size_t maxlen);
//void							perror(const char *);

#endif
