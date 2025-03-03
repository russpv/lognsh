#include "lex_int.h"

/* Loads a char into buf.
 * Checks for the closing single quote. advances ptr and Returns 1 if found
 * without incrementing ptr.
 * Stops buffer overflow.
 */
static inline int	_load_buf(t_lex *lexer)
{
	debug_print(_MOD_": %s, ptr:_%c_\n", __FUNCTION__, *lexer->ptr);
	if ((unsigned char)OP_SQUOTE == *lexer->ptr)
	{
		lexer->ptr++;
		return (debug_print(_MOD_": found closing quote\n"), 1);
	}
	else
	{
		if (lexer->buf_idx < LEX_BUFSZ - 1)
		{
			if ((unsigned char)OP_NULL == *lexer->ptr)
				return (debug_print(_MOD_": found null before closing quote\n"), 1);
			lexer->buf[(lexer->buf_idx)] = *lexer->ptr;
			(lexer->buf_idx)++;
		}
		else
			return (print_lex_buffer_overflow(), 1);
	}
	return (0);
}

/* Expected to add only one token to the llist at most
 * Assumes ptr is on the first single quote
 * If '\'' not found, flags incomplete only
 * Ignores missing closing quote.
 * Creates token only if normal delims found
 */
int	tokenize_single_quotes(t_lex *lexer)
{
	t_tok	*token;

	debug_print(_MOD_": STATE: %s, ptr:_%c_\n", __FUNCTION__, *lexer->ptr);
	while (++lexer->ptr)
		if (1 == _load_buf(lexer))
			break ;
	if ((unsigned char)OP_NULL == *lexer->ptr)
	{
		debug_print(_MOD_": WARNING: FOUND NULL\n");
		lexer->input_incomplete = true;
	}
	token = lex_create_token(lexer, TOK_WORD);
	if (NULL == token)
		return (1);
	if (0 != add_token(lexer, token))
		return (1);
	lexer->ptr++;
	return (0);
}

/* terminates a lexer run */
int	tokenize_null(t_lex *lexer)
{
	t_tok	*token;

	debug_print(_MOD_": %s\n", __FUNCTION__);
	if (lexer)
	{
		token = create_token("\0", TOK_EOF, (size_t)(lexer->ptr
					- lexer->raw_string));
		if (token)
			add_token(lexer, token);
		else
			return (1);
	}
	return (0);
}
