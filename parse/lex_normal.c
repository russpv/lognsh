#include "lex_int.h"

/* */
static bool	_is_normal_delim(unsigned char s)
{
	debug_print( "_is_normal_delim:_%c_", s);
	if (ft_strchr(NORMALDELIMS, s))
	{
		debug_print( "YES\n");
		return (true);
	}
	debug_print( "NO\n");
	return (false);
}

/* A subset of normal delimiters to trigger state 
 * transition
 */
static bool	_is_transition_char(unsigned char s)
{
	debug_print( "_is_transition_char:_%c_", s);
	if (s != 0 && ft_strchr(NORMALTRANSITIONS, s))
	{
		debug_print( "YES\n");
		return (true);
	}
	debug_print( "NO\n");
	return (false);
}

static int	_word_or_name(const char *s)
{
	debug_print( "_word_or_name:_%s_", s);
	if (!*s)
	{
		debug_print( "ERR\n");
		return (TOK_ERR);
	}
	while (*s)
	{
		if (!(ft_isalnum(*s) || '_' == *s))
		{
			debug_print( "WORD\n");
			return (TOK_WORD);
		}
		s++;
	}
	debug_print( "NAME\n");
	return (TOK_NAME);
}


/* Looks one char ahead to detect matches 
 * that are superstrings of substrings, e.g. '||' vs '|'
 * Updates lexer state if successful
 * Pointer is already on next char
 */
static struct s_ht_entry	*_do_one_char_lookahead(t_lex *lexer, struct s_ht_entry	*res)
{
	const size_t buflen = ft_strlen(lexer->buf);
	char *s;

	s = lexer->ptr;
	if (!lexer->ptr || !*(s) || 0 == buflen)
		return (NULL);
	lexer->buf[buflen] = *(s);
	debug_print("_do_one_char_lookahead with:_%s_\n", lexer->buf);
	struct s_ht_entry *test = ht_lookup(lexer->hasht, lexer->buf);
	if (test)
	{
		lexer->ptr++;
		return (test);
	}
	else	
		lexer->buf[buflen] = 0;
	return (res);
}

// Use a hashtable on the tokens where entries identify 100% overlap
// if overlap flag, 1-char lookahead and recheck hashtable
// else return word or name using a helper func
// modifies input string location
/* Returns a new token of chars when points to a delimiter */
static t_tok	*_match_normal(t_lex *lexer)
{
	debug_print( "_match_normal\n");
	struct s_ht_entry *res = NULL;
	const char *start = lexer->ptr;

	while (' ' == *lexer->ptr)
	{
		start++;
		lexer->ptr++;
	}
	if (true == _is_transition_char(*lexer->ptr))
		return (NULL);
	while (*lexer->ptr && false == _is_normal_delim(*lexer->ptr))
	{	
		if ((unsigned char)TK_ESC == *lexer->ptr)
		{
			lexer->escape_mode = true;
			++lexer->ptr;
		}
		if ((unsigned char)OP_ENV == *lexer->ptr && false == lexer->escape_mode)
			lexer->do_expansion = DO_EXPANSION;
		if ((unsigned char)OP_STAR == *lexer->ptr && false == lexer->escape_mode)
			lexer->do_globbing = DO_GLOBBING;
		lexer->escape_mode = false;
		lexer->buf[lexer->ptr - start] = *lexer->ptr;
		lexer->ptr++;
   		fprintf(stderr, "Buffer content: '%s'\n", lexer->buf);  // Expecting a string with a single character
		res = ht_lookup(lexer->hasht, lexer->buf);
		if (res)
		{
			if (true == ((t_ht_data)(ht_get_payload(res)))->is_substring)
				res = _do_one_char_lookahead(lexer, res);	
			return (lex_create_token(lexer, ((t_ht_data)ht_get_payload(res))->type));
		}
	}
	if (true == _is_normal_delim(*lexer->ptr) && ft_strlen(lexer->buf) > 0)
		return (lex_create_token(lexer, _word_or_name(lexer->buf)));
	if ((unsigned char)OP_NULL == *lexer->ptr) // if delim is EOF
	{
		debug_print("##### FOUND NULL\n");
		return (NULL);
	}
	if (true == _is_normal_delim(*lexer->ptr)) // check if operator
	{
		lexer->buf[0] = *lexer->ptr++;
		res = ht_lookup(lexer->hasht, lexer->buf);
		if (res)
		{
			if (true == ((t_ht_data)(ht_get_payload(res)))->is_substring)
				res = _do_one_char_lookahead(lexer, res);	
			return (lex_create_token(lexer, ((t_ht_data)ht_get_payload(res))->type));
		}
	}
	return (NULL);
}

/* Expected to add multiple tokens to the llist 
 * Adds token to llist when ptr advances to a 
 * delimiter char, which includes transition chars.
 * 
*/
int	tokenize_normal(t_lex *lexer)
{
	debug_print( "tokenize_normal\n");
	while (lexer->ptr && !_is_transition_char(*lexer->ptr))
	{
		t_tok *token = _match_normal(lexer);
		debug_print( "ptr at:_%c_\n", *lexer->ptr);
		if (token)
			if (0 != add_token(lexer, token))
				return (1);
		if (OP_NULL == *lexer->ptr)
		{
			debug_print( "##### TOK EOF FOUND\n");
			return (0);
		}

	}
	return (0);
}
