#include "lex_int.h"

static int	_word_or_name(const char *s)
{
	if (!*s)
		return (TOK_ERR);
	while (*s)
	{
		if (!(ft_isalnum(*s) || '_' == *s))
			return (TOK_WORD);
		s++;
	}
	return (TOK_NAME);
}


/* Looks one char ahead to detect matches 
 * that are superstrings of substrings, e.g. '||' vs '|'
 * Updates lexer state if successful
 */
static struct s_ht_entry	*_do_one_char_lookahead(t_lex *lexer, struct s_ht_entry	*res)
{
	const size_t buflen = ft_strlen(lexer->buf);
	char *s;

	s = lexer->ptr;
	if (!lexer->ptr || !*(s + 1) || 0 == buflen)
		return (NULL);
	lexer->buf[buflen] = *(s + 1);
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
static t_tok	*_match_normal(t_lex *lexer)
{
	int i  = 0;
	struct s_ht_entry *res = NULL;
	const char *start = lexer->ptr;
	char *s = lexer->ptr;

	while (' ' == *s)
		s++;
	while (*s && false == is_normal_delim(*s))
	{	
		if ((unsigned char)TK_ESC == *s)
		{
			lexer->escape_mode = true;
			++s;
		}
		if ((unsigned char)OP_ENV == lexer->ptr && false == lexer->escape_mode)
			lexer->do_expansion = DO_EXPANSION;
		if ((unsigned char)OP_STAR == lexer->ptr && false == lexer->escape_mode)
			lexer->do_globbing = DO_GLOBBING;
		lexer->escape_mode = false;
		lexer->buf[lexer->ptr - start - 1] = *s++;
		res = ht_lookup(lexer->hasht, lexer->buf);
		if (res)
		{
			if (true == ((t_ht_data)(ht_get_payload(res)))->is_substring)
				res = _do_one_char_lookahead(lexer, res);	
			return (lex_create_token(lexer->buf, ((t_ht_data)ht_get_payload(res))->type));
		}
	}
	if (true == is_normal_delim(*s))
		return (lex_create_token(lexer, word_or_name(lexer->buf)));
	if ((unsigned char)TOK_EOF == *s)
		return (lex_create_token(lexer, TOK_EOF));
	return (NULL);
}

/* Expected to add multiple tokens to the llist */
int	tokenize_normal(t_lex *lexer)
{
	const char *start = lexer->ptr;

	while (lexer->ptr && !is_transition_char(lexer->ptr))
	{
		t_tok *token = _match_normal(lexer);
		if (token)
			if (0 != add_token(lexer, token))
				return (1);
		if (token && token->type == TOK_EOF)
			return (0);
		else if (!token)
			err();
	}
	return (0);
}
