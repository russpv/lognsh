#include "lex_int.h"

/* Returns true if closing quote found at l->ptr 
** (and pops stack, skips char)
** checks also for $ transition
*/
static bool	_is_dquote_transition_delim(t_lex *l)
{
	debug_print(_MOD_ ": -------- %s:_%c_", __FUNCTION__, *l->ptr);
	if (*l->ptr == OP_NULL)
	{
		st_int_pop(l->stack);
		l->state = ON_EOF;
		return (debug_print(" YES-NULL\n"), true);
	}
	if (ft_strchr(NORMALTRANSITIONS, *l->ptr))
	{
		if (OP_DQUOTE == *l->ptr)
		{
			debug_print(" FOUND CLOSING QUOTE\n");
			st_int_pop(l->stack);
			l->ptr++;
			return (debug_print(" YES\n"), true);
		}
		else if (l->ptr != NULL && OP_NULL != *l->ptr)
		{
			if (is_dollar_delim(l))
				return (debug_print(" YES\n"), true);
		}
	}
	debug_print(" NO\n");
	return (false);
}

/* Note: add special parameters here */
static inline t_tok	*_process_dquote_logic(t_state *s, t_lex *lexer)
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
		st_int_pop(lexer->stack);
		lexer->state = ON_EOF;
		return (lex_create_token(get_mem(s), lexer, TOK_WORD));
	}
	return (NULL);
}

/* Returns next (sub)token, loads buf, doesn't flush buf
 * if unescaped $, record it in the token record for later expansion
 * if bs\, skip it IF next char is bs\, $, double quote
 * if normal loop doesn't find closing ", flags incomplete
 * if nothing on buf at transition exit, tokenizes null raw TOK_WORD
 */
static inline t_tok	*_match_double(t_state *s, t_lex *lexer)
{
	t_tok	*token;

	debug_print(_MOD_ ": %s: _%c_\n", __FUNCTION__, *lexer->ptr);
	token = NULL;
	if (lexer->ptr)
	{
		lexer->ptr++;
		while (*lexer->ptr && !_is_dquote_transition_delim(lexer))
		{
			token = _process_dquote_logic(s, lexer);
			lexer->escape_mode = false;
			if (token)
				return (token);
			debug_print(_MOD_ ":     Got:_%c_\n", *lexer->ptr);
			put_on_buf(lexer);
		}
		lexer->input_incomplete = true;
		//if (0 == ft_strlen(lexer->buf))
		//	return (NULL);
		if (false == is_normal_delim(lexer, 0))
			lexer->is_subtoken = true;
		token = lex_create_token(get_mem(s), lexer, TOK_WORD);
	}
	return (token);
}

/* Can add multiple tokens to llist
 * Uses stack to remember unclosed quotes
 * state transition manages pop
 * Ptr starts on first double quote
 *
 * Searches rest of string until " found, or
 * a valid dollar expansion
 * else, flag incomplete
 */
int	tokenize_double_quotes(t_state *s, t_lex *lexer)
{
	t_tok	*token;

	debug_print(_MOD_ YELLOW ": STATE: %s, ptr:_%c_\n" RESET, __FUNCTION__,
		*lexer->ptr);
	if (lexer)
	{
		token = _match_double(s, lexer);
		if (!token)
		{
			return (0);
		}
		debug_print(_MOD_ ": ptr at _%c_\n", *lexer->ptr);
		if (0 != add_token(get_mem(s), lexer, token))
			return (ERR_GENERAL);
	}
	return (0);
}
