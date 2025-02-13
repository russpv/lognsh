#include "lex_int.h"

// Process escape sequences in the lexer
void	process_escape_sequence(t_lex *l)
{
	if ((unsigned char)TK_ESC == *l->ptr)
	{
		l->escape_mode = true;
		l->ptr++;
	}
}

// Sets flags only for special operators like environment variables and globbing
// Advance the lexer pointer past comments
int	process_special_operators(t_lex *lexer)
{
	if ((unsigned char)OP_ENV == *lexer->ptr && false == lexer->escape_mode)
		lexer->do_expansion = true;
	if ((unsigned char)OP_STAR == *lexer->ptr && false == lexer->escape_mode)
		lexer->do_globbing = true;
	if ((unsigned char)OP_HASH == *lexer->ptr && false == lexer->escape_mode)
	{
		lexer->escape_mode = false;
		while (*lexer->ptr && *lexer->ptr != TK_NEWL && *lexer->ptr != TK_EOF)
			lexer->ptr++;
		return (1);
	}
	lexer->escape_mode = false;
	return (0);
}
