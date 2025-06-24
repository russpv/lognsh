#include "lex_int.h"

char	*lex_get_ptr(t_lex *lexer)
{
	if (!lexer)
		return (NULL);
	return (lexer->ptr);
}

const char	*lex_get_raw(t_lex *lexer)
{
	if (!lexer)
		return (NULL);
	return (lexer->raw_string);
}

char	*lex_get_eof(t_lex *lexer)
{
	if (!lexer)
		return (NULL);
	return (lexer->eof_word);
}

unsigned int	lex_get_keepd(t_lex *lexer)
{
	if (!lexer)
		return (-1);
	return (lexer->keep_dollar);
}

bool	lex_get_incomplete(t_lex *lexer)
{
	if (!lexer)
		return (false);
	return (lexer->input_incomplete);
}

int	lex_get_lines(t_lex *lexer)
{
	if (!lexer)
		return (-1);
	return (lexer->lines);
}

t_list	*lex_get_tokens(t_lex *lexer)
{
	if (!lexer)
		return (NULL);
	return (lexer->token_list);
}
