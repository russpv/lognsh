#include "lex_int.h"

/* Expected to add only one token to the llist 
 * Assumes ptr is on the single quote
 */
int	tokenize_single_quotes(t_lex *lexer)
{
	debug_print("tokenize_single_quotes, ptr:_%c_\n", *lexer->ptr);
	const char *start = ++lexer->ptr;
	t_tok *token;

	while (lexer->ptr)
	{
		debug_print("ptr:_%c_\n", *lexer->ptr);
		if ((unsigned char)OP_SQUOTE == *lexer->ptr)
		{
			debug_print( "FOUND CLOSING QUOTE\n");
			break;
		}
		else
			lexer->buf[lexer->ptr - start - 1] = *lexer->ptr++;
	}
	if ((unsigned char)TOK_EOF == *lexer->ptr)
	{
		debug_print("FOUND EOF\n");
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
		lexer->ptr++; // move past closing quote
	}
	return (0);
}

/* Returns next token, loads buf, doesn't flush buf
 * if $, record it in the token record for later expansion
 * if bs, skip it if next char is bs, dollar, double quote or backtick
 */
static t_tok	*_match_double(t_lex *lexer)
{
	debug_print("_match_double\n");
	const char *start = lexer->ptr;
	t_tok	*token = NULL;

	if (lexer->ptr)
	{
		while (*(++lexer->ptr))
		{
			if ((unsigned char)TK_ESC == *lexer->ptr)
			{
				if ((unsigned char)TK_ESC == *(lexer->ptr + 1) || \
					(unsigned char)OP_ENV == *(lexer->ptr + 1) || \
					(unsigned char)OP_DQUOTE == *(lexer->ptr + 1) || \
					(unsigned char)OP_BACKTICK == *(lexer->ptr + 1))
				{
					lexer->escape_mode = true;
					++lexer->ptr;
				}
			}
			if ((unsigned char)OP_ENV == *lexer->ptr && false == lexer->escape_mode)
				lexer->do_expansion = DO_EXPANSION;
			if ((unsigned char)OP_DQUOTE == *lexer->ptr && false == lexer->escape_mode)
				return(lex_create_token(lexer, TOK_WORD));
			lexer->escape_mode = false;
			debug_print("    Got:_%c_\n", *lexer->ptr);
			lexer->buf[lexer->ptr - start - 1] = *lexer->ptr;
		}
	}
	return (token);
}

/* Expected to add only one token to the llist 
 * Ptr starts on first double quote
 */
int	tokenize_double_quotes(t_lex *lexer)
{
	debug_print("tokenize_double_quotes\n");
	/* search rest of string until " found else err
	 * if $, record it in the token record for later expansion
	 * if bs, skip it if next char is bs, dollar, double quote or backtick
	 * everything else is not a delimiter
	 */
	if (lexer)
	{
		t_tok *token = _match_double(lexer); // Did not find closing quote
		if (!token)
			return (1);
		debug_print("created token\n");
		lexer->ptr++; //move past the closing quote
		debug_print("ptr at _%c_\n", *lexer->ptr);
		if (0 != add_token(lexer, token))
			return (1);
	}
	return (0);
}

/* terminates a lexer run */
int	tokenize_null(t_lex *lexer)
{
	debug_print("tokenize_null\n");
	if (lexer)
	{
		t_tok *token = create_token("\0", TOK_EOF, lexer->ptr - lexer->raw_string);
		if (token)
			add_token(lexer, token);
		else
			return (1);
	}
	return (0);
}