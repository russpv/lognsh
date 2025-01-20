#include "lex_int.h"

/* Check for the closing single quote or EOF. Returns 1 if found. */
static inline int	_load_buf(t_lex *lexer, int *buf_idx)
{
	debug_print("ptr:_%c_\n", *lexer->ptr);
	if ((unsigned char)OP_SQUOTE == *lexer->ptr)
	{
		debug_print("FOUND CLOSING QUOTE\n");
		return (1);
	}
	else
		lexer->buf[(*buf_idx)++] = *lexer->ptr;
	return (0);
}

/* Expected to add only one token to the llist
 * Assumes ptr is on the first single quote
 */
int	tokenize_single_quotes(t_lex *lexer)
{
	int		buf_idx;
	t_tok	*token;

	debug_print("tokenize_single_quotes, ptr:_%c_\n", *lexer->ptr);
	buf_idx = 0;
	while (++lexer->ptr)
		if (1 == _load_buf(lexer, &buf_idx))
			break ;
	if ((unsigned char)TOK_EOF == *lexer->ptr)
	{
		debug_print("ERR FOUND EOF\n");
		return (1);
	}
	else
	{
		token = lex_create_token(lexer, TOK_WORD);
		debug_print("created token\n");
		if (NULL == token)
			return (1);
		if (0 != add_token(lexer, token))
			return (1);
		lexer->ptr++;
	}
	return (0);
}

/* terminates a lexer run */
int	tokenize_null(t_lex *lexer)
{
	t_tok	*token;

	debug_print("tokenize_null\n");
	if (lexer)
	{
		token = create_token("\0", TOK_EOF, lexer->ptr - lexer->raw_string);
		if (token)
			add_token(lexer, token);
		else
			return (1);
	}
	return (0);
}
