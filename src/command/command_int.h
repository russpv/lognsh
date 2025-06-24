#ifndef COMMAND_INT_H
# define COMMAND_INT_H

# include "command.h"
# define _MOD_ "Command"
# define NO_CMD -10

/*
** Expansion operations are performed in this order:
**
** 1) Parameter expansion: $VAR is replaced with its ENV value.
** 	No fancy stuff. Also, $? specials do their thing.
** 2) Command substitutions are not supported.
** 3) Word splitting: checks results of expansions to split words by IFS
** 4) Filename expansion: if word has unquoted ‘*’, it is replaced
** 	with custom lexicographic 'casefolded' sorted list
** 	of matching (case sensitive) filenames in pwd. If
** 	no matches, the word is left unchanged. Starting dots
** 	must be matched explicitly.
** 5) Pattern matching: * Matches any string, including the null string
*/

/* Holds context for current command */
struct					s_cmd
{
	t_int_stack			*st;
	char				*fullpath;

	int					argc;

	char				**argv;
	int					argvc;

	t_list				*redirs;
	int					redc;

	t_ast_node			*curr_node;
	int					curr_cmdc;

	int					**fildes;
	int					saved_stdout;
	int					saved_stdin;
	int					saved_stderr;
};

// Used to bypass func limits for creating argv
typedef struct s_cmd	t_cmd;

struct					s_context
{
	t_state				*s;
	t_ast_node			*a;
	char				**args;
	char				**argv;
	const char			*cmdname;
	int					argc;
	int					count;
};

void					destroy_cmd(t_mem_mgr *m, void **c);
t_cmd					*init_cmd(t_state *s, t_ast_node *a);

int						c_argstoargv(t_state *s, t_cmd *cmd, t_ast_node *a,
							char **args);

int						run_cmd(t_state *s, t_ast_node *a);

int						cmd_exec_simple(t_state *s, t_ast_node *a);
int						cmd_execute_full(t_state *s, t_ast_node *a);
int						cmd_exec_pipe(t_state *s, t_ast_node *pipe);
int						cmd_exec_log(t_state *s, t_ast_node *a);
int						cmd_exec_proc(t_state *s, t_ast_node *a);

int						proc_args_redirs(t_state *s, t_ast_node *a, t_cmd *c);
int						find_and_validate_cmd(t_state *s, const char *name,
							char **fullpath);

void					print_pipes(t_cmd *c);

#endif
