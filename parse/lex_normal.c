#include "lex_int.h"

static t_tok	*_match_normal_token(t_lex *lexer, int *buf_idx)
{
	t_tok	*res;

	res = NULL;
	while (*lexer->ptr && false == is_normal_delim(*lexer->ptr))
	{
		process_escape_sequence(lexer);
		if (1 == process_special_operators(lexer))
			continue ;
		lexer->buf[(*buf_idx)++] = *lexer->ptr++;
		debug_print("------Buffer content: '%s'\n", lexer->buf);
		res = lex_ht_lookup(lexer);
		if (res)
			return (res);
	}
	return (NULL);
}

static t_tok	*_match_word_or_name(t_lex *lexer)
{
	if (true == is_normal_delim(*lexer->ptr) && ft_strlen(lexer->buf) > 0)
		return (lex_create_token(lexer, word_or_name(lexer->buf)));
	return (NULL);
}

// check if operator
static t_tok	*_match_normal_op(t_lex *lexer)
{
	t_tok	*res;

	res = NULL;
	if (true == is_normal_delim(*lexer->ptr))
	{
		lexer->buf[0] = *lexer->ptr++;
		res = lex_ht_lookup(lexer);
		if (res)
			return (res);
	}
	return (NULL);
}

// Use a hashtable on the tokens where entries identify 100% overlap
// if overlap flag, 1-char lookahead and recheck hashtable
// else return word or name using a helper func
// modifies input string location
/* Returns a new token of chars when points to a delimiter */
static t_tok	*_match_normal(t_lex *lexer)
{
	t_tok	*res;
	int		buf_idx;

	debug_print("----_match_normal\n");
	res = NULL;
	buf_idx = 0;
	while (' ' == *lexer->ptr)
		lexer->ptr++;
	if (true == is_transition_char(*lexer->ptr))
		return (NULL);
	res = _match_normal_token(lexer, &buf_idx);
	if (res)
		return (res);
	res = _match_word_or_name(lexer);
	if (res)
		return (res);
	if ((unsigned char)OP_NULL == *lexer->ptr)
	{
		debug_print("----FOUND NULL\n");
		return (NULL);
	}
	res = _match_normal_op(lexer);
	if (res)
		return (res);
	return (NULL);
}

/* Expected to add multiple tokens to the llist
 * Adds token to llist when ptr advances to a
 * delimiter char, which includes transition chars.
 *
 */
int	tokenize_normal(t_lex *lexer)
{
	t_tok	*token;

	debug_print("tokenize_normal\n");
	while (lexer->ptr && !is_transition_char(*lexer->ptr))
	{
		token = _match_normal(lexer);
		debug_print("ptr at:_%c_\n", *lexer->ptr);
		if (token)
			if (0 != add_token(lexer, token))
				return (1);
		if (OP_NULL == *lexer->ptr)
		{
			debug_print("##### TOK EOF FOUND\n");
			return (0);
		}
	}
	return (0);
}
