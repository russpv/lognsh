#include "lex_int.h"

static int	_add_subtoken(t_lex *lexer, t_tok *subtok)
{
	assert(NULL != lexer->last_grp_tok);
	tok_add_subtok(lexer->last_grp_tok, subtok);
	tok_incr_tokc(lexer->last_grp_tok);
	return (0);
}

/* Adds to llist tail a new token, clears buf */
int	add_token(t_lex *lexer, t_tok *token)
{
	debug_print(_MOD_ ": %s\n", __FUNCTION__);
	if (token && lexer)
	{
		memset(lexer->buf, 0, LEX_BUFSZ);
		lexer->buf_idx = 0;
		if (lexer->tokc < LEX_MAX_TOKC)
		{
			if (lexer->last_grp_tok)
			{
				debug_print(_MOD_ ": %s adding subtoken to grp\n", __FUNCTION__);
				_add_subtoken(lexer, token);
			}
			else
			{
				debug_print(_MOD_ ": %s adding token to list\n", __FUNCTION__);
				ft_lstadd_back(&lexer->token_list, ft_lstnew(token));
				lexer->tokc++;
			}
			debug_print(_MOD_": %s checking normal delimiter ptr:%c\n", __FUNCTION__, *lexer->ptr);
			if (lexer->last_grp_tok && is_normal_delim(*lexer->ptr, lexer->ptr + 1))
			{
				debug_print(_MOD_": %s adding grp token to list\n", __FUNCTION__);
				ft_lstadd_back(&lexer->token_list, ft_lstnew(lexer->last_grp_tok));
				lexer->is_subtoken = false;
				lexer->last_grp_tok = NULL;
				lexer->tokc++;
			}
		}
		else 
			return (ERR_BUFFLOW);
		ft_lstsize(lexer->token_list);
		return (0);
	}
	debug_print(_MOD_ ": %s GOT FUCKED\n", __FUNCTION__);
	return (ERR_ARGS);
}

/* Creates token based on current buf and ptr, does not add to llist 
 * Resets buf and buf_idx and tokenizing flags.
 * Looks ahead for normal delim to reset subtoken flag.
 * Note: skip any terminator that is not to be part of token (\")
 * Note: is_subtoken is turned off here only.
 */
t_tok	*lex_create_token(t_lex *lexer, int type)
{
	t_tok	*token;
	t_tok	*grp_token;

	debug_print(_MOD_": %s\n", __FUNCTION__);
	if (!lexer)
		return (NULL);
	if (true == lexer->is_subtoken && NULL == lexer->last_grp_tok)
	{
		grp_token = create_token(lexer->buf, TOK_GROUP_WORD, (size_t)(lexer->ptr \
			- lexer->raw_string));
		if (!grp_token)
			return (err(ERRMSG_MALLOC), NULL);
		lexer->last_grp_tok = grp_token;
		debug_print(_MOD_": %s: Created GROUP token\n", __FUNCTION__);
	}
	token = create_token(lexer->buf, type, (size_t)(lexer->ptr \
				- lexer->raw_string));
	if (token)
	{
		if (true == lexer->is_subtoken)
			tok_set_subtoken(token);
		if (IN_DOUBLE_QUOTES == lexer->state)
			tok_set_dquotes(token);
		if (true == lexer->do_globbing)
		{
			tok_set_globbing(token);
			tok_set_globbing(lexer->last_grp_tok);
		}
		if (true == lexer->do_expansion)
		{
			tok_set_expansion(token);
			tok_set_expansion(lexer->last_grp_tok);
		}
		lexer->do_globbing = INITVAL;
		lexer->do_expansion = INITVAL;
		debug_print(_MOD_": %s: Created token\n", __FUNCTION__);
	}
	return (token);
}

inline bool	is_too_long(const char *input)
{
	if (NULL == input)
		return (true);
	if (ft_strlen(input) >= LEX_BUFSZ)
	{
		debug_print("Lexer: ERROR: Input exceeds buf size.\n");
		return (true);
	}
	return (false);
}

bool	lex_get_incomplete(t_lex *lexer)
{
	return (lexer->input_incomplete);
}

t_list	*lex_get_tokens(t_lex *lexer)
{
	return (lexer->token_list);
}

bool	is_varnamechar(unsigned char c)
{
	debug_print(_MOD_ ": %s: %c?\n", __FUNCTION__, c);
	return (ft_isalnum((int)c) || '_' == c);
}

/* VARNAMELEN
** This returns length of the string pointed to by 'c'
** As long as it is a shell variable name
** NOT including '\0' (e.g. 'size')
** Will NOT SEGV if c is NULL (UNlike strlen())
*/
size_t	ft_varnamelen(const char *c)
{
	size_t	i;

	i = 0;
	if (!c)
		return (0);
	if (ft_isdigit(c[0]))
		return (0);
	while (c[i] && is_varnamechar(c[i]))
		i++;
	return (i);
}

// Tests for '$' followed by correct char
bool	is_dollar_delim(unsigned char c, char *next)
{
	if (!next)
		return (false);
	return ('$' == c && (is_varnamechar(*next) || '?' == *next));
}

/* A subset of normal delimiters to trigger state
 * transition. If "<<" , must change state
 */
bool	is_transition_delim(unsigned char s, char *next)
{
	debug_print(_MOD_": -------- %s:_%c_", __FUNCTION__, s);
	if (s == 0)
		return (debug_print(" YES-NULL\n"), true);
	if (ft_strchr(NORMALTRANSITIONS, s))
	{
		if (s == '<')
		{
			if (next)
				if ('<' == *next)
					return (debug_print(" YES\n"), true);
		} 
		else if (is_dollar_delim(s, next)) 
			return (debug_print(" YES\n"), true);
		else
			return (debug_print(" YES\n"), true);
	}
	debug_print(" NO\n");
	return (false);
}

// If char s-next sequence matches any NORMALDELIMS
// Looks ahead for any doubles, '&&'
bool	is_normal_delim(unsigned char s, char *next)
{
	debug_print(_MOD_": -------- %s:_%c%c_", __FUNCTION__, s, next);
	if (ft_strchr(NORMALDELIMS, s))
	{
		if ('&' == s)
		{
			if (next)
				if ('&' == *next)
					return (debug_print(" YES\n"), true);
		} 
		debug_print(" YES\n");
		return (true);
	}
	debug_print(" NO\n");
	return (false);
}

// Puts input char on buf, and increments idx and ptr
int	put_on_buf(t_lex *l)
{
	debug_print(_MOD_": %s: _%c_\n", __FUNCTION__, *l->ptr);
	if (l->buf_idx < LEX_BUFSZ - 1)
		l->buf[(l->buf_idx)++] = *l->ptr++;
	else
		return (print_lex_buffer_overflow(), ERR_BUFFLOW);	
	return (0);
}
