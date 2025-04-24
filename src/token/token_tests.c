
#include "token_int.h"

bool	tok_get_issubtoken(t_tok *token)
{
	if (!token)
		return (false);
	if (GROUP == token->class)
		return (false);
	return (true == token->t.tok.is_subtoken);
}

bool	tok_isgrouptoken(t_tok *token)
{
	if (!token)
		return (false);
	return (GROUP == token->class);
}
