#include "lex_int.h"

t_tok *lex_ht_lookup(t_lex *lexer)
{
	struct s_ht_entry *res = NULL;

	res = ht_lookup(lexer->hasht, lexer->buf);
	if (res)
	{
		if (true == ((t_ht_data)(ht_get_payload(res)))->is_substring)
			res = do_one_char_lookahead(lexer, res);	
		if (true == is_normal_delim(*lexer->ptr))
			return (lex_create_token(lexer, ((t_ht_data)ht_get_payload(res))->type));
	}
	return (NULL);
}

/* Looks two chars ahead to detect matches 
 * that are superstrings of substrings, e.g. '||' vs '|'
 * with a word boundary following next char
 * Updates lexer state if successful
 * Pointer is already on next char
 */
struct s_ht_entry	*do_one_char_lookahead(t_lex *lexer, struct s_ht_entry	*res)
{
	const size_t buflen = ft_strlen(lexer->buf);
	char *s;

	s = lexer->ptr;
	if (!lexer->ptr || !*(s) || 0 == buflen)
		return (NULL);
	lexer->buf[buflen] = *(s);
	debug_print("------_do_one_char_lookahead with:_%s_\n", lexer->buf);
	struct s_ht_entry *test = ht_lookup(lexer->hasht, lexer->buf);
	if (test)
	{
		if (true == is_normal_delim(*(lexer->ptr + 1)))
		{
			lexer->ptr++;
			return (test);
		}
	}
	else	
		lexer->buf[buflen] = 0;
	return (res);
}
