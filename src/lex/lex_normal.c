#include "lex_int.h"

/* Handles current char, loads buf and advances */
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

/* Handles '$?' here */
static t_tok	*_match_word_or_name_or_exception(t_lex *lexer, int *buf_idx)
{
    if (true == is_dollar_question(lexer))
    {
        debug_print("----_match_word_or_name: got $? \n");
		lexer->buf[(*buf_idx)++] = *lexer->ptr++;
        return lex_create_token(lexer, TOK_EXIT_STATUS);
    }
	if (true == is_normal_delim(*lexer->ptr) && ft_strlen(lexer->buf) > 0)
	{
		debug_print("----_match_word_or_name: got %s\n", lexer->buf);
		return (lex_create_token(lexer, word_or_name(lexer->buf)));
	}
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
		debug_print("----_match_normal_op: got _%s_\n", lexer->buf);
		res = lex_ht_lookup(lexer);
		if (res)
		{
			debug_print("----_match_normal_op: match %s\n", lexer->buf);
			return (res);
		}
	}
	return (NULL);
}

// Use a hashtable on the tokens where entries identify 100% overlap
// if overlap flag, 1-char lookahead and recheck hashtable
// else return word or name using a helper func
// modifies input string location
/* Returns a new token of chars when points to a delimiter 
 */
static t_tok	*_match_normal(t_lex *lexer)
{
	t_tok	*res;
	int		buf_idx;

	debug_print("----_match_normal\n");
	res = NULL;
	buf_idx = 0;
	while (' ' == *lexer->ptr)
		lexer->ptr++;
	if (true == is_transition_char(lexer, *lexer->ptr))
		return (NULL);
	res = _match_normal_token(lexer, &buf_idx);
	if (res)
		return (res);
	res = _match_word_or_name_or_exception(lexer, &buf_idx);
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
	debug_print("----_match_normal DONE\n");
	return (NULL);
}

/* Expected to add multiple tokens to the llist
 * Adds token to llist when ptr advances to a
 * delimiter char, which includes transition chars.
 */
int	tokenize_normal(t_lex *lexer)
{
	t_tok	*token;

	debug_print("tokenize_normal\n");
	while (lexer->ptr && !is_transition_char(lexer, *lexer->ptr))
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
