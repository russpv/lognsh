#ifndef PARSE_H
# define PARSE_H

#include "../data_structures/stack.h"
#include "../data_structures/llist.h"
#include "../state/state.h"
#include "../lex/lex.h"

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

enum						e_ast_node_type
{
	NONE,
	AST_NODE_LOG,
	AST_NODE_PIPELINE,
	AST_NODE_CMD,
	AST_NODE_PROC
};


typedef struct s_global_state	t_state;
typedef struct s_node t_ast_node;

t_ast_node	*parse(t_state *s, char *input);

int	p_get_type(t_ast_node *a);
char *p_get_cmd(t_ast_node *a);
char **p_get_argv(t_ast_node *a);

/*  Expansions */
// tilde ~ ?
// $VAR
// $(...) ?

/* Globbing */
// * wildcard only
// OR defer till execution if commands create files

/* delimiter handling */
// single vs double quotes
// Parentheses
// preserve literal strings

/* Error handling */
// Undefined variables cannot be expanded
// Invalid globbing pattern
// Unmatched quotation marks? >> Says throw ERR.

#endif
