#include "token_int.h"

bool	tok_get_globbing(t_tok *token)
{
	return (token->do_globbing);
}

bool	tok_get_expansion(t_tok *token)
{
	return (token->do_expansion);
}

bool	tok_get_dquotes(t_tok *token)
{
	return (token->in_dquotes);
}
