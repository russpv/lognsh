
#include "lex_int.h"

/* True if s is in the set of tokens that do not need to be delimited */
static inline bool	_is_not_delimd(const char *s)
{
	if (ft_strlen(s) == 1)
		if (ft_strchr(NOTDELIMITED, *s))
			return (true);
	if (0 == ft_strcmp(s, "<<"))
		return (true);
	if (0 == ft_strcmp(s, ">>"))
		return (true);
	if (0 == ft_strcmp(s, "||"))
		return (true);
	if (0 == ft_strcmp(s, "&&"))
		return (true);
	return (false);
}

/* Returns hashtable matches (buf) by tokenizing current buf */
t_tok	*lex_ht_lookup(t_state *s, t_lex *lexer)
{
	struct s_ht_entry	*res;
	void				*elem;

	if (ft_strnlen(lexer->buf, LEX_BUFSZ - 1) == 0)
		return (NULL);
	res = ht_lookup(lexer->hasht, lexer->buf);
	if (res)
	{
		if (lexer->ptr)
			dprint(_MOD_ ": Found hasht match. buf: %c\n", *lexer->buf);
		if (true == ((t_ht_data)(ht_get_payload(res)))->is_substring)
			res = do_one_char_lookahead(lexer, res);
		if (true == is_normal_delim(lexer, 0)
			|| true == _is_not_delimd(lexer->buf))
		{
			dprint(_MOD_ ": Creating token...\n");
			elem = ht_get_payload(res);
			if (elem)
				return (lex_create_token(get_mem(s), lexer,
						((t_ht_data)elem)->type));
		}
	}
	return (NULL);
}

/* Looks two chars ahead to detect matches
 * that are superstrings of substrings, e.g. '||' vs '|'
 * with a word boundary following next char
 * Updates lexer state if successful
 * Pointer is already on next char
 * Returns res without modification
 */
struct s_ht_entry	*do_one_char_lookahead(t_lex *lexer, struct s_ht_entry *res)
{
	const size_t		buflen = ft_strlen(lexer->buf);
	char				*s;
	struct s_ht_entry	*test;

	s = lexer->ptr;
	if (!lexer->ptr || !*(s) || 0 == buflen)
		return (res);
	lexer->buf[buflen] = *(s);
	dprint(_MOD_ ": ------ %s with:_%s_\n", __FUNCTION__, lexer->buf);
	test = ht_lookup(lexer->hasht, lexer->buf);
	if (test && test != res)
	{
		if (true == is_normal_delim(lexer, 0) || _is_not_delimd(lexer->buf))
		{
			dprint(_MOD_ ": ------ %s found:_%s_\n", __FUNCTION__, lexer->buf);
			lexer->ptr++;
			return (test);
		}
	}
	else
		lexer->buf[buflen] = 0;
	return (res);
}
