#include "tokens_int.h"

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

size_t	tok_get_pos(t_tok *token)
{
	if (!token)
		return (-1);
	if (GROUP == token->class)
		return (token->t.meta.pos);
	else
		return (token->t.tok.pos);
}

// Returns list of subtokens, if a group token
t_list	*tok_get_tlist(t_tok *token)
{
	if (!token)
		return (NULL);
	if (GROUP != token->class)
		return (NULL);
	return (token->t.meta.tokens);
}

char	*tok_get_raw(t_tok *token)
{
	if (!token)
		return (NULL);
	if (GROUP == token->class)
		return (GRP_TOK_RAW);
	return (token->t.tok.raw);
}

enum e_tok_type	tok_get_type(t_tok *token)
{
	if (GROUP == token->class)
		return (TOK_GROUP_WORD);
	return (token->t.tok.type);
}

size_t	tok_get_len(t_tok *token)
{
	if (!token)
		return (-1);
	if (NORMAL == token->class)
		return (token->t.tok.raw_len);
	return (-1);
}
