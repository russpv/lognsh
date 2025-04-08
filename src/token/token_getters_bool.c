#include "token_int.h"

bool	tok_get_globbing(t_tok *token)
{
	if (!token)
		return (NULL);
	if (GROUP == token->class)
		return (token->t.meta.do_globbing);
	return (token->t.tok.do_globbing);
}

bool	tok_get_expansion(t_tok *token)
{
	if (!token)
		return (NULL);
	if (GROUP == token->class)
		return (token->t.meta.do_expansion);
	return (token->t.tok.do_expansion);
}

bool	tok_get_dquotes(t_tok *token)
{
	if (!token)
		return (err("No Token\n"), false);
	if (GROUP == token->class)
		return (token->t.meta.in_dquotes);
	return (token->t.tok.in_dquotes);
}
