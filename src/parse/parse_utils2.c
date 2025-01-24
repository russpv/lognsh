#include "parse_int.h"

bool	is_arg_token(t_tok *tok)
{
	const enum e_tok_type	type = tok_get_type(tok);

	return (type == TOK_WORD || type == TOK_NAME);
}

void	parse_print(t_ast_node *ast)
{
	(void)ast;
	// TODO call printer methods
	// ast_traverse
}

