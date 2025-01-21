#include "../data_structures/llist.h"
#include "parse.h"

t_ast	parse(t_state s, char *input)
{
	t_lex	*lexer;

	// work with example: [builtcmd 'arg' && cmd -arg -arg "$VAR" > fn]
	lexer = tokenize(input);
	if (NULL == lexer)
		return (null_and_stat(s, ERR_TOKEN));
	// TODO make AST and parse
	// create_ast(lexer);
	return (NULL);
}

// For debugging
void	parse_print(t_ast ast)
{
	// TODO
	// ast_traverse
}
