#include "lex_int.h"


// What we need to do:
// tokenize a subtoken when we have reached a transition char without a space before it.

// At last, tokenizes any transition chars (operators) found in ht
// Assumes buf is empty
static t_tok	*_match_normal_op(t_state *s, t_lex *lexer)
{
	t_tok	*res;

	res = NULL;
	debug_print(_MOD_": ---- %s\n",__FUNCTION__);
	if (true == is_normal_delim(*lexer->ptr, (lexer->ptr + 1)))
	{
		lexer->buf[0] = *lexer->ptr++;
		debug_print(_MOD_": ---- %s: got _%s_\n", __FUNCTION__, lexer->buf);
		res = lex_ht_lookup(s, lexer);
		if (res)
		{
			debug_print(_MOD_": ---- %s: match %s\n", __FUNCTION__, lexer->buf);
			return (res);
		}
	}
	return (NULL);
}

/* Tokenizes words and subwords. Called by iterator when delim reached.
 * Handles: '$?', $VARs, and whatever remains in buf.
 * If buf was empty, implies a transition char, does nothing. 
 * If transition char reached instead of delim, tokenizes subword.
 * Note: is_dollar_question() works because '?' is a delim
 */
static t_tok	*_match_word_or_name_or_exception(t_state *s, t_lex *lexer)
{
	debug_print(_MOD_": ---- %s: got %s\n", __FUNCTION__, lexer->buf);

	if (true == is_dollar_question(lexer)) // move to ENVVAR?
	{
		lexer->buf[(lexer->buf_idx)++] = *lexer->ptr++;
		return (lex_create_token(s, lexer, TOK_EXIT_STATUS));
	}
	else if (true == is_normal_delim(*lexer->ptr, (lexer->ptr + 1))
		&& ft_strlen(lexer->buf) > 0)
	{
		return (lex_create_token(s, lexer, word_or_name(lexer->buf)));
	}

	if (true == is_transition_delim(*lexer->ptr, (lexer->ptr + 1))
		&& ft_strlen(lexer->buf) > 0)
	{ 
		lexer->is_subtoken = true;
		return (lex_create_token(s, lexer, word_or_name(lexer->buf)));
	}
	return (NULL);
}

/* 
 * Advances on input until delim or transition char.
 * Leaves tokenization for downstream unless word found in ht.
 */
static t_tok	*_match_reserved_token(t_state *s, t_lex *lexer)
{
	t_tok	*res;

	res = NULL;
	debug_print(_MOD_": ---- %s\n",__FUNCTION__);
	while (*lexer->ptr \
		&& false == is_normal_delim(*lexer->ptr, (lexer->ptr + 1)) \
		&& false == is_transition_delim(*lexer->ptr, (lexer->ptr + 1)))
	{
		process_escape_sequence(lexer);
		if (SKIPREST == process_special_operators(lexer)) // remove env var stuff
			continue ;
		lexer->buf[(lexer->buf_idx)++] = *lexer->ptr++;
		debug_print(_MOD_": ------ Buf: '%s' idx:%d_\n", lexer->buf, lexer->buf_idx);
		res = lex_ht_lookup(s, lexer);
		if (res)
			return (res);
	}
	return (NULL);
}

// Use a hashtable on the tokens where entries identify 100% overlap
// if overlap flag, 1-char lookahead and recheck hashtable
// else return word or name using a helper func
// modifies input string location
/* Returns a token when points to a delimiter
 */
static t_tok	*_match_normal(t_state *s, t_lex *lexer)
{
	t_tok	*res;

	debug_print(_MOD_": ---- %s\n",__FUNCTION__);
	res = NULL;
	while (' ' == *lexer->ptr || '\t' == *lexer->ptr) //skip discard delims
		lexer->ptr++;
	if (true == is_transition_delim(*lexer->ptr, (lexer->ptr + 1)))
		return (NULL);
	res = _match_reserved_token(s, lexer);
	if (res)
		return (res);
	res = _match_word_or_name_or_exception(s, lexer);
	if (res)
		return (res);
	if ((unsigned char)OP_NULL == *lexer->ptr)
		return (debug_print(_MOD_": ----FOUND NULL\n"), NULL);
	res = _match_normal_op(s, lexer);
	if (res)
		return (res);
	debug_print(_MOD_": ---- %s DONE\n", __FUNCTION__);
	return (NULL);
}

/* Expected to add multiple tokens to the llist
 * Adds token to llist when ptr advances to a
 * delimiter char or transition char. Tokenizes transition operator.
 */
int	tokenize_normal(t_state *s, t_lex *lexer)
{
	t_tok	*token;

	debug_print(_MOD_ YELLOW": STATE: %s\n"RESET,__FUNCTION__);
	while (lexer->ptr && !is_transition_delim(*lexer->ptr, (lexer->ptr + 1)))
	{
		token = _match_normal(s, lexer);
		debug_print(_MOD_": ptr at:_%c_\n", *lexer->ptr);
		if (token)
			if (0 != add_token(lexer, token))
				return (ERR_GENERAL);
		if (OP_NULL == *lexer->ptr)
		{
			debug_print(_MOD_": ##### TOK EOF FOUND\n");
			return (0);
		}
	}
	return (0);
}
