#include "parse_int.h"

bool	is_open_paren(t_tok *tok)
{
	return (tok_get_type(tok) == TOK_OPEN_PAREN);
}

bool	is_close_paren(t_tok *tok)
{
	return (tok_get_type(tok) == TOK_CLOSE_PAREN);
}
