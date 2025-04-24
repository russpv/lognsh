
// command.h
#ifndef COMMAND_H
# define COMMAND_H

# include "../execute/execute.h"
# include "../parse/parse.h"
# include "../state/state.h"

/* COMMAND ADT
 * Sequences execution
 * Uses Strategy pattern to delegate atomic cmds to Execute
 * does not manage global shell state or env, hist, multi-cmd
 */

/*
** 1) If name has no slashes, searches built-ins, or path
** 2) If search unsuccessful, returns code 127.
** 3) If search successful, or has slashes, executes in a child
** 4) Arg 0 is set to the command name
** 5) Waits for child to exit and keeps exit status.
**
** Note: unlike Bash, searches built-ins first before path
** Does not cache search results in hashtable. Does not invoke
** command_not_found_handle function (returns 127).
** Does not execute shell scripts.
*/

enum								e_exec_context
{
	CTXT_PROC,
	CTXT_CMD,
	CTXT_PIPELINE,
	CTXT_LOG,
};

typedef struct s_cmd				t_cmd;
typedef struct s_global_state		t_state;
typedef struct s_node				t_ast_node;

typedef struct s_command_functions	t_cmd_fns;

t_cmd_fns							*init_cmd_fns(t_state *s);
void								destroy_cmd_fns(t_mem_mgr *m, t_cmd_fns *f);

char								*c_get_fullpath(t_cmd *c);
char								**c_get_argv(t_cmd *c);
t_int_stack							*c_get_ctxtst(t_cmd *c);
t_ast_node							*c_get_node(t_cmd *c);
int									c_get_cmdc(const t_cmd *c);
int									c_get_argvc(const t_cmd *c);
int									c_get_redc(const t_cmd *c);

t_list								*c_get_redirs(const t_cmd *c);

const int							**c_get_fildes(const t_cmd *c);

int									cmd_execute(t_state *s, t_ast_node *a);

int									save_redirs(t_cmd *c);
int									restore_redirs(t_cmd *c);

#endif
