#ifndef COMMAND_INT_H
# define COMMAND_INT_H

# include "command.h"

/* Holds context for current command */
struct					s_cmd
{
	t_stack *st; // parse depth
	char				*fullpath;
	char				**argv;
	int					argc;
	t_ast_node			*curr_node;
	int					**fildes;
	int					curr_cmdc;
	int					saved_stdout;
	int					saved_stdin;
	int					saved_stderr;
};

typedef struct s_cmd	t_cmd;

char					**c_argstoargv(char **args, char *cmd, int argc);

int						run_cmd(t_state *s, t_ast_node *a);

int						cmd_exec_simple(t_state *s, t_ast_node *a);
int						cmd_execute_full(t_state *s, t_ast_node *a);
int						cmd_exec_pipe(t_state *s, t_ast_node *pipe);
int						cmd_exec_log(t_state *s, t_ast_node *a);
int						cmd_exec_proc(t_state *s, t_ast_node *a);

#endif
