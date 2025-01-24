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
typedef struct s_node t_ast_node;
t_ast_node	*parse(t_state *s, char *input);

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
