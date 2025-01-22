#include "lex_int.h"

static inline t_tok	*_process_dquote_logic(t_lex *lexer)
{
	if ((unsigned char)TK_ESC == *lexer->ptr)
	{
		if ((unsigned char)TK_ESC == *(lexer->ptr + 1)
			|| (unsigned char)OP_ENV == *(lexer->ptr + 1)
			|| (unsigned char)OP_DQUOTE == *(lexer->ptr + 1)
			|| (unsigned char)OP_BACKTICK == *(lexer->ptr + 1))
		{
			lexer->escape_mode = true;
			++lexer->ptr;
		}
	}
	if ((unsigned char)OP_ENV == *lexer->ptr && false == lexer->escape_mode)
		lexer->do_expansion = DO_EXPANSION;
	if ((unsigned char)OP_DQUOTE == *lexer->ptr && false == lexer->escape_mode)
		return (lex_create_token(lexer, TOK_WORD));
	return (NULL);
}

/* Returns next token, loads buf, doesn't flush buf
 * if $, record it in the token record for later expansion
 * if bs, skip it if next char is bs, dollar, double quote or backtick
 * if normal loop doesn't find closing ", flags incomplete
 */
static inline t_tok	*_match_double(t_lex *lexer)
{
	t_tok	*token;
	int		buf_idx;

	debug_print("_match_double\n");
	token = NULL;
	buf_idx = 0;
	if (lexer->ptr)
	{
		while (*(++lexer->ptr))
		{
			token = _process_dquote_logic(lexer);
			if (token)
				return (token);
			lexer->escape_mode = false;
			debug_print("    Got:_%c_\n", *lexer->ptr);
			lexer->buf[buf_idx++] = *lexer->ptr;
		}
		lexer->is_incomplete = true;
		token = lex_create_token(lexer, TOK_WORD);
	}
	return (token);
}

/* Expected to add only one token to the llist
 * Ptr starts on first double quote
 *
 * Searches rest of string until " found
 * else, flag incomplete
 * if $, record it in the token record for later expansion
 * if bs, skip it if next char is bs, dollar, double quote or backtick
 * everything else is not a delimiter
 */
int	tokenize_double_quotes(t_lex *lexer)
{
	t_tok	*token;

	debug_print("tokenize_double_quotes\n");
	if (lexer)
	{
		token = _match_double(lexer);
		if (!token)
			return (1);
		debug_print("created token\n");
		lexer->ptr++;
		debug_print("ptr at _%c_\n", *lexer->ptr);
		if (0 != add_token(lexer, token))
			return (1);
	}
	return (0);
}
