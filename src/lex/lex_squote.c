#include "lex_int.h"

/* Loads a char into buf. 
 * Checks for the closing single quote. Returns 1 if found
 * without incrementing ptr.
 * Stops buffer overflow.
 */
static inline int	_load_buf(t_lex *lexer, int *buf_idx)
{
	debug_print("Lexer: _load_buf... ptr:_%c_\n", *lexer->ptr);
	if ((unsigned char)OP_SQUOTE == *lexer->ptr)
	{
		debug_print("Lexer: tokenize_single_quotes: found closing quote\n");
		return (1);
	}
	else
	{
		if (*buf_idx < LEX_BUFSZ - 1)
		{
			if ((unsigned char)OP_NULL == *lexer->ptr)
			{
				debug_print("Lexer: tokenize_single_quotes: found null before closing quote\n");
				return (1);
			}
			lexer->buf[(*buf_idx)] = *lexer->ptr;
			(*buf_idx)++;
		}
		else
		{
			print_lex_buffer_overflow();
			return (1);
		}
	}
	return (0);
}

/* Expected to add only one token to the llist
 * Assumes ptr is on the first single quote
 * If '\'' not found, flags incomplete only
 * Ignores missing closing quote.
 */
int	tokenize_single_quotes(t_lex *lexer)
{
	int		buf_idx;
	t_tok	*token;

	debug_print("Lexer: tokenize_single_quotes... ptr:_%c_\n", *lexer->ptr);
	buf_idx = 0;
	while (++lexer->ptr)
		if (1 == _load_buf(lexer, &buf_idx))
			break ;
	if ((unsigned char)OP_NULL == *lexer->ptr)
	{
		debug_print("Lexer: WARNING: FOUND NULL\n");
		lexer->is_incomplete = true;
	}
	token = lex_create_token(lexer, TOK_WORD);
	debug_print("Lexer: tokenize_single_quotes: created token\n");
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

	debug_print("Lexer: tokenize_null...\n");
	if (lexer)
	{
		token = create_token("\0", TOK_EOF, (size_t)(lexer->ptr - lexer->raw_string));
		if (token)
			add_token(lexer, token);
		else
			return (1);
	}
	return (0);
}
