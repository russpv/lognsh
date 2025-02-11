#include "parse_int.h"

/* Added builtins so that they can be considered
 * args. Ex: exec echo haha, or cat echo
 */
bool	is_arg_token(t_tok *tok)
{
	const enum e_tok_type	type = tok_get_type(tok);

	return (type == TOK_WORD || type == TOK_NAME || type == TOK_ENV_VAR
		|| type == TOK_EXIT_STATUS || type == TOK_BI);
}

/* Returns true if tok's raw string is an OP_OPTION (-) */
bool	is_option(t_tok *tok)
{
	const char	*s = tok_get_raw(tok);

	if (s && *s)
		return ((unsigned char)OP_OPTION == s[0]);
	return (false);
}

/* True if token type needs to be expanded.
 */
bool	is_expansion(t_tok *tok)
{
	const enum e_tok_type	type = tok_get_type(tok);

	return (type == TOK_EXIT_STATUS || type == TOK_ENV_VAR
		|| true == tok_get_expansion(tok));
}

bool	is_globbing(t_tok *tok)
{
	return (true == tok_get_globbing(tok));
}
