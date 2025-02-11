// Low-level implementation of forking, piping, redirects
#ifndef EXECUTE_H
# define EXECUTE_H

# include "../builtins/bi.h"
# include "../command/command.h"
# include "../state/state.h"
# include <stdbool.h>
# include <sys/types.h>
# include <sys/wait.h>

/* Duplicate declares */
typedef struct s_node	t_ast_node;

typedef int				(*t_execute_fn)(t_state *s, t_ast_node *node);

int						redirect(int *to, char *topath, int from,
							bool ifappend);
int						exec_create_pipes(int ***fildes, int cmd_count);

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

int						get_exit_status(int status);
void					waitchild(int *status, int childc);

void					free_pipes(int **fildes, int count);

#endif
