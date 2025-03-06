#include "lex_int.h"

static bool	_is_dquote_transition_delim(t_lex *l)
{
	debug_print(_MOD_": -------- %s:_%c_", __FUNCTION__, *l->ptr);
	if (*l->ptr == 0)
		return (debug_print(" YES-NULL\n"), true);
	if (ft_strchr(NORMALTRANSITIONS, *l->ptr))
	{
		if (l->ptr != NULL && '\0' != *l->ptr) 
		{
			if (is_dollar_delim(*l->ptr, (l->ptr + 1))) 
				return (debug_print(" YES\n"), true);
		}
	}
	debug_print(" NO\n");
	return (false);
}

//TODO process specials
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
	if ((unsigned char)OP_DQUOTE == *lexer->ptr && false == lexer->escape_mode)
	{
		lexer->ptr++;
		return (lex_create_token(lexer, TOK_WORD)); //creates final token
	}
	return (NULL);
}

/* Returns next (sub)token, loads buf, doesn't flush buf
 * if unescaped $, record it in the token record for later expansion
 * if bs, skip it IF next char is bs, dollar, double quote (or backtick)
 * if normal loop doesn't find closing ", flags incomplete
 * note: single quotes are literal, thus only \" or \0 trigger state change
 */
static inline t_tok	*_match_double(t_lex *lexer)
{
	t_tok	*token;

	debug_print(_MOD_": %s\n", __FUNCTION__);
	token = NULL;
	if (lexer->ptr)
	{
		lexer->ptr++;
		while (*lexer->ptr && !_is_dquote_transition_delim(lexer))
		{
			token = _process_dquote_logic(lexer);
			lexer->escape_mode = false;
			if (token)
				return (token);
			debug_print(_MOD_":     Got:_%c_\n", *lexer->ptr);
			put_on_buf(lexer);
		}
		lexer->input_incomplete = true;
		if (0 == ft_strlen(lexer->buf))
			return (NULL);
		if (false == is_normal_delim(*lexer->ptr, (lexer->ptr + 1)))
			lexer->is_subtoken = true;
		token = lex_create_token(lexer, TOK_WORD); // tokenize subtoken
	}
	return (token);
}

/* Expected to add only one token to the llist
 * Ptr starts on first double quote
 *
 * Searches rest of string until " found, or 
 * a valid dollar expansion
 * else, flag incomplete
 */
int	tokenize_double_quotes(t_lex *lexer)
{
	t_tok	*token;

	debug_print(_MOD_ YELLOW": STATE: %s, ptr:_%c_\n"RESET, __FUNCTION__, *lexer->ptr);
	if (lexer)
	{
		token = _match_double(lexer);
		if (!token)
			return (0); //empty dquote
		//lexer->ptr++;
		debug_print(_MOD_": ptr at _%c_\n", *lexer->ptr);
		if (0 != add_token(lexer, token))
			return (ERR_GENERAL);
	}
	return (0);
}
