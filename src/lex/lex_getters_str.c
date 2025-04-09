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
