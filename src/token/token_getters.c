#include "token_int.h"

bool	tok_get_globbing(t_tok *token)
{
	if (GROUP == token->class)
		return (token->t.meta.do_globbing);
	return (token->t.tok.do_globbing);
}

bool	tok_get_expansion(t_tok *token)
{
	if (GROUP == token->class)
		return (token->t.meta.do_expansion);
	return (token->t.tok.do_expansion);
}

bool	tok_get_dquotes(t_tok *token)
{
	return (token->t.tok.in_dquotes);
}

bool	tok_get_issubtoken(t_tok *token)
{
	return (GROUP != token->class);
}

bool	tok_isgrouptoken(t_tok *token)
{
	return (GROUP == token->class);
}

// Returns list of subtokens, if a group token
t_list	*tok_get_tlist(t_tok *token)
{
	if (GROUP != token->class)
		return (NULL);
	return (token->t.meta.tokens);
}
