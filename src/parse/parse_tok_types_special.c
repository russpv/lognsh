
#include "parse_int.h"

bool	is_group_token(t_tok *tok)
{
	const enum e_tok_type	type = tok_get_type(tok);

	return (type == TOK_GROUP_WORD);
}

/* True if token type needs to be expanded.
 */
bool	is_expansion(t_tok *tok)
{
	const enum e_tok_type	type = tok_get_type(tok);

	return (type == TOK_EXIT_STATUS || type == TOK_ENV_VAR
		|| true == tok_get_expansion(tok));
}

bool	is_group_op_token(t_tok *tok)
{
	const enum e_tok_type	type = tok_get_type(tok);

	return (type == TOK_PIPE || type == TOK_ANDIF || type == TOK_ORIF);
}

bool	is_globbing(t_tok *tok)
{
	return (true == tok_get_globbing(tok));
}
