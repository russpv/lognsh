#ifndef PARSE_H
# define PARSE_H

# include "../../include/libft.h"
# include "../data_structures/llist.h"
# include "../data_structures/stack.h"
# include "../lex/lex.h"
# include "../state/state.h"
# include "../token/token.h"
# include <dirent.h>
# include <sys/types.h>

/* PARSE
 * Converts user input into an AST
 * Tokenizes input, Expands variables, Does globbing(?)
 *
 * cmd: echo $HOME/ *.c | grep foo && echo "done"
 *
 * input: 'echo $HOME/ *.c'
 * tokens: "echo", "$HOME", "/ *.c"
 * expanded: "echo", "/home/user", "/ *.c"
 * globbed: ["echo", "/home/user/main.c", "/home/user/utils.c"]
 *
 * AND
 * ├── PIPE
 * │   ├── CMD [echo, /home/user/main.c, /home/user/utils.c]
 * │   └── CMD [grep, foo]
 * └── CMD [echo, done]
 */

// Notes: Parse will access State for expansions

// Builds the AST (Composite Pattern) where commands are leaf
// nodes; pipes, conditionals and args are elsewhere
// Command module interprets (processes) AST

enum							e_ast_node_type
{
	NONE,
	AST_NODE_LOG,
	AST_NODE_PIPELINE,
	AST_NODE_CMD,
	AST_NODE_PROC
};

typedef struct s_global_state	t_state;
typedef struct s_node			t_ast_node;
typedef struct s_redir			t_redir_data; //Exposed for logging only

t_parse_fns *init_parse_fns(t_state *s);

t_ast_node						*parse(t_state *s, char *input);

int								p_get_type(t_ast_node *a);
char							*p_get_cmd(t_ast_node *a);
t_list							**p_get_args(t_ast_node *a);
t_list							*p_get_redirs(t_ast_node *a);
int								p_get_argc(t_ast_node *a);
int								p_get_redc(t_ast_node *a);
bool							p_get_expansion(t_ast_node *a);
bool							p_get_grouptok(t_ast_node *a);
bool						p_get_has_redgrouptoks(t_ast_node *n);

char							*p_get_fn(const t_redir_data *r);

/* Command getters */
t_list							*p_get_proc_cmds(t_ast_node *a);
int								p_get_proc_cmdc(t_ast_node *a);
t_list							*p_get_pipe_cmds(t_ast_node *a);
int								p_get_pipe_cmdc(t_ast_node *a);
t_list							*p_get_log_cmds(t_ast_node *a);
int								p_get_log_cmdc(t_ast_node *a);
t_list							*p_get_log_ops(t_ast_node *a);

/* These methods are called by Command */
int								p_do_arg_processing(t_state *s, t_ast_node *a,
									char ***args);
int								p_do_redir_processing(t_state *s,
									t_ast_node *a);
int								p_do_redirections(t_ast_node *a);

int								p_set_cmd(t_mem_mgr *m, t_ast_node *a,
									const char *cmd);
int								p_update_argc(t_ast_node *a, int amt);

void							destroy_redir(t_mem_mgr *m, void **in);

bool							is_globbing(t_tok *tok);

void							print_ast(t_ast_node *ast, int depth);
void							debug_print_list(t_list *head);

#endif
