#include "lex_int.h"

/* Expected to add only one token to the llist */
int	tokenize_single_quotes(t_lex *lexer)
{
	const char *start = lexer->ptr;

	while (++lexer->ptr)
	{
		if ((unsigned char)OP_SQUOTE == *lexer->ptr)
			break;
		else
			lexer->buf[lexer->ptr - start - 1] = *lexer->ptr;
	}
	if ((unsigned char)TOK_EOF == *lexer->ptr)
		return (1);
	else
		add_token(lexer, TOK_WORD);
	return (0);
}

/* Returns next token, doesn't flush buf
 * if $, record it in the token record for later expansion
 * if bs, skip it if next char is bs, dollar, double quote or backtick
 */
static t_tok	*_match_double(t_lex *lexer)
{
	const char *start = lexer->ptr;
	t_tok	*token = NULL;

	if (lexer->ptr)
	{
		while (++lexer->ptr)
		{
			if ((unsigned char)TK_ESC == lexer->ptr)
			{
				if ((unsigned char)TK_ESC == lexer->ptr + 1 || \
					(unsigned char)OP_ENV == lexer->ptr + 1 || \
					(unsigned char)OP_DQUOTE == lexer->ptr + 1 || \
					(unsigned char)OP_BACKTICK == lexer->ptr + 1)
				{
					lexer->escape_mode = true;
					++lexer->ptr;
				}
			}
			if ((unsigned char)OP_ENV == lexer->ptr && false == lexer->escape_mode)
				lexer->do_expansion = DO_EXPANSION;
			if ((unsigned char)OP_DQUOTE == lexer->ptr && false == lexer->escape_mode)
				return(create_token(lexer, TOK_WORD, lexer->ptr - start));
			lexer->escape_mode = false;
			lexer->buf[lexer->ptr - start - 1] = lexer->ptr;
		}
	}
	return (token);
}

/* Expected to add only one token to the llist */
int	tokenize_double_quotes(t_lex *lexer)
{
	/* search rest of string until " found else err
	 * if $, record it in the token record for later expansion
	 * if bs, skip it if next char is bs, dollar, double quote or backtick
	 * everything else is not a delimiter
	 */
	if (lexer)
	{
		t_tok *token = _match_double(lexer);
		if (!token)
			return (1);
		if (0 != add_token(lexer, token))
			return (1);
	}
	return (0);
}
