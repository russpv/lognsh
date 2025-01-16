#include "parse.h"

# define FAIL_TOKEN 10 //TODO group somewhere else

/* PARSE
 * Returns AST object from user string input
 * Does variable expansions with t_state
 */
t_ast	parse(t_state s, char *input)
{
	// work with example: [builtcmd 'arg' && cmd -arg -arg "$VAR" > fn]
	t_ll llist; //TODO define this
	if (0 != tokenize(input, &llist))
		return (null_and_stat(s, FAIL_TOKEN));

}

// For debugging
void	parse_print(t_ast ast)
{
	//TODO
	// ast_traverse
}

