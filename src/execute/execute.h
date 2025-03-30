// Low-level implementation of forking, piping, redirects
#ifndef EXECUTE_H
# define EXECUTE_H

# include "../builtins/bi.h"
# include "../command/command.h"
# include "../state/state.h"
# include <stdbool.h>
# include <sys/types.h>
# include <sys/wait.h>
# include <sys/stat.h>

/* Duplicate declares */
typedef struct s_node	t_ast_node;

typedef int				(*t_execute_fn)(t_state *s, t_ast_node *node);
typedef int				(*t_builtin_fn)(t_state *s, char **args, int argc);

int						redirect(int *to, char *topath, int from,
							bool ifappend);
int						exec_create_pipes(t_mem_mgr *m, int ***fildes,
							int cmd_count);

/* Atomic commands */
int						exec_fork_execve(t_state *s);
int						exec_bi_call(t_state *s, t_builtin_fn bi);

/* Higher level commands */
int						exec_fork_wait(t_state *s, t_ast_node *node,
							t_execute_fn executor);
int						exec_pipe_fork_redirect_run(t_state *s,
							t_ast_node *node, int i, t_execute_fn executor);
int						exec_fork_run(t_state *s, t_ast_node *node, int i,
							t_execute_fn executor);

int						exec_close_pipes(int **fildes, int cmd_count);

int	exec_get_exit_status(int status);
int						waitchild(int *status, int childc);
int	waitchild_sigint(int *status, pid_t child_pid);

int	exec_heredoc(t_mem_mgr *m, t_lex *l);

#endif
