#ifndef EXECUTE_H
# define EXECUTE_H

# include "../builtins/bi.h"
# include "../data_structures/stack.h"
# include "../state/state.h"
# include <stdbool.h>
# include <sys/stat.h>
# include <sys/types.h>
# include <sys/wait.h>

/*
** EXECUTE
** Low-level implementation of forking, piping, redirects
**
** 1) If no command given, first word resulting from expansion is
** 	taken as the command name, remaining are args.
** 2) Redirections are performed. A redirection error causes the
** 	command to exit with a non-zero status.
**
** Note: Exit status is that of the last command in the input
*/

/* Duplicate declares */
typedef struct s_node	t_ast_node;

typedef int				(*t_execute_fn)(t_state *s, t_ast_node *node);
typedef int				(*t_builtin_fn)(t_state *s, char **args, int argc);

typedef struct s_exec
{
	int					cmdc;
	t_execute_fn		executor;
	t_cmd				*proc_cmd;
	t_ast_node			*proc;
	pid_t				*pids;
	int					*exit_codes;
}						t_exec;

t_exec					*exec_init(t_mem_mgr *m, int cmdc, t_cmd *c,
							t_ast_node *a);
void					exec_set_executor(t_exec *e, t_execute_fn x);
void					destroy_exec(t_mem_mgr *m, t_exec *e);

int						redirect(int *to, char *topath, int from,
							bool ifappend);
int						exec_create_pipes(t_mem_mgr *m, int ***fildes,
							int cmd_count);

/* Atomic commands */
int						exec_fork_execve(t_state *s);
int						exec_bi_call(t_state *s, t_builtin_fn bi,
							t_cmd_fns *cf);

/* Higher level commands */
int						exec_fork_redirect_wait(t_state *s, t_ast_node *node,
							t_exec *e, t_cmd_fns *cf);
int						exec_pipe_fork_redirect_run(t_state *s,
							t_ast_node *node, int i, t_exec *e);
int						exec_fork_run(t_state *s, t_ast_node *node, int i,
							t_execute_fn executor);

int						exec_close_pipes(int **fildes, int cmd_count);

int						exec_get_exit_status(int status);
int						waitchilds(int *status, int childc);
int						waitchild_sigint(int *status, pid_t child_pid);
int						waitchildpid(int *status, pid_t p);
int						waitchildspid(int *status, t_exec *e);

int						exec_heredoc(t_state *s, t_lex *l);

#endif
