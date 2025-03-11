#include "lex_int.h"

#define NOTDELIMITED "()<"

/* Checks if s is in the set of tokens that do not need to be delimited */
static inline bool	_is_not_delimd(const char *s)
{
	if (ft_strlen(s) > 1)
		return (false);
	if (ft_strchr(NOTDELIMITED, *s))
		return (true);
	return (false);
}

/* Returns hashtable matches by tokenizing current buf */
t_tok	*lex_ht_lookup(t_state *s, t_lex *lexer)
{
	struct s_ht_entry	*res;

	res = NULL;
	res = ht_lookup(lexer->hasht, lexer->buf);
	if (res)
	{
		debug_print("Lexer: Found hasht match. Ptr: %c\n", *lexer->ptr);
		if (true == ((t_ht_data)(ht_get_payload(res)))->is_substring)
			res = do_one_char_lookahead(lexer, res);
		if (true == is_normal_delim(*lexer->ptr, (lexer->ptr + 1)) \
			|| true == _is_not_delimd(lexer->buf))
		{
			debug_print("Lexer: Creating token...\n");
			return (lex_create_token(get_mem(s), lexer,
					((t_ht_data)ht_get_payload(res))->type));
		}
	}
	return (NULL);
}

/* Looks two chars ahead to detect matches
 * that are superstrings of substrings, e.g. '||' vs '|'
 * with a word boundary following next char
 * Updates lexer state if successful
 * Pointer is already on next char
 */
struct s_ht_entry	*do_one_char_lookahead(t_lex *lexer, struct s_ht_entry *res)
{
	const size_t		buflen = ft_strlen(lexer->buf);
	char				*s;
	struct s_ht_entry	*test;

	s = lexer->ptr;
	if (!lexer->ptr || !*(s) || 0 == buflen)
		return (NULL);
	lexer->buf[buflen] = *(s);
	debug_print("Lexer: ------_do_one_char_lookahead with:_%s_\n", lexer->buf);
	test = ht_lookup(lexer->hasht, lexer->buf);
	if (test)
	{
		if (true == is_normal_delim(*(lexer->ptr + 1), (lexer->ptr + 2)))
		{
			lexer->ptr++;
			return (test);
		}
	}
	else
		lexer->buf[buflen] = 0;
	return (res);
}
