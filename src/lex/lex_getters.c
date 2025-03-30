#include "lex_int.h"

unsigned int	lex_get_keepd(t_lex *lexer)
{
	if (!lexer)
		return (-1);
	return (lexer->keep_dollar);
}


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
