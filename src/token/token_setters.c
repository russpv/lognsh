#include "token_int.h"

int	tok_set_globbing(t_tok *token)
{
	token->do_globbing = true;
	debug_print("Token: tok_set_globbing: %s_ glob_%d \n", tok_get_raw(token),
		tok_get_globbing(token));
	return (0);
}

int	tok_set_dqoute(t_tok *token)
{
	token->in_dquotes = true;
	return (0);
}

int	tok_set_expansion(t_tok *token)
{
	token->do_expansion = true;
	debug_print("Token: tok_set_expansion: %s_ glob_%d \n", tok_get_raw(token),
		tok_get_expansion(token));
	return (0);
}

int	tok_set_dquotes(t_tok *token)
{
	token->in_dquotes = true;
	return (0);
}
