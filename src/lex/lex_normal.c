#include "lex_int.h"

// true if ptr on a command delimiter
// (e.g. all the sequences that delimit a redirection)
bool	on_cmd_op(t_lex *l)
{
	const char	*s = l->ptr;

	if (!s)
		return (false);
	if (*s == OP_HASH || *s == OP_PIPE || *s == OP_OPENP || *s == OP_CLOSEDP
		|| *s == '&')
		return (true);
	else if (*s == OP_REDOUT || *s == OP_REDIN)
	{
		if ((*s == OP_REDOUT && *(l->ptr + 1) == OP_REDOUT) || (*s == OP_REDIN
				&& *(l->ptr + 1) == OP_REDIN))
			return (true);
	}
	return (false);
}

// Returns true if buf[0] is '$' and ptr on '?'
bool	is_dollar_question(t_lex *lexer)
{
	return (lexer->buf[0] == '$' && *(lexer->ptr) == '?');
}

// returns TOK_WORD if alphanumeric or underscore, else TOK_NAME
int	word_or_name(const char *s)
{
	dvprint(_MOD_ ": -------- %s:_%s_", __FUNCTION__, s);
	if (!*s)
	{
		dvprint(" ERR\n");
		return (TOK_ERR);
	}
	while (*s)
	{
		if (!(ft_isalnum(*s) || '_' == *s))
		{
			dvprint(" WORD\n");
			return (TOK_WORD);
		}
		s++;
	}
	dvprint(" NAME\n");
	return (TOK_NAME);
}

// Process escape sequences in the lexer
void	process_escape_sequence(t_lex *l)
{
	if ((unsigned char)TK_ESC == *l->ptr)
	{
		l->escape_mode = true;
		l->ptr++;
	}
}

// Sets flags only for special operators like environment variables and globbing
// Advance the lexer pointer past comments #
int	process_special_operators(t_lex *lexer)
{
	unsigned char	prev;

	if ((unsigned char)OP_ENV == *lexer->ptr && ft_varnamelen((lexer->ptr + 1))
		&& false == lexer->escape_mode)
		lexer->do_expansion = true;
	if ((unsigned char)OP_EQUALS == *lexer->ptr && false == lexer->escape_mode)
		lexer->is_assignment = true;
	if ((unsigned char)OP_STAR == *lexer->ptr && false == lexer->escape_mode)
		lexer->do_globbing = true;
	if ((unsigned char)OP_HASH == *lexer->ptr && false == lexer->escape_mode)
	{
		prev = is_valid_prev(lexer, 0);
		if (' ' == prev)
		{
			dprint(_MOD_ ": -------- %s got ' #', skipping...\n",
				__FUNCTION__);
			lexer->escape_mode = false;
			while (*lexer->ptr && *lexer->ptr != TK_NEWL
				&& *lexer->ptr != TK_EOF)
				lexer->ptr++;
			return (SKIPREST);
		}
	}
	lexer->escape_mode = false;
	return (0);
}

// At last, tokenizes any transition chars (operators) found in ht
// Redirects found here
// Assumes buf is empty
static t_tok	*_match_normal_op(t_state *s, t_lex *lexer)
{
	t_tok	*res;

	res = NULL;
	dprint(_MOD_ ": ---- %s\n", __FUNCTION__);
	if (true == is_normal_delim(lexer, 0))
	{
		lexer->buf[0] = *lexer->ptr++;
		dprint(_MOD_ ": ---- %s: got _%s_\n", __FUNCTION__, lexer->buf);
		res = lex_ht_lookup(s, lexer);
		if (res)
		{
			dprint(_MOD_ ": ---- %s: ht-matched %s\n", __FUNCTION__,
				lexer->buf);
			return (res);
		}
	}
	return (NULL);
}

/* Tokenizes words and subwords. Called by iterator when delim reached.
 * If buf was empty, implies a transition char, does nothing.
 * If transition char reached instead of delim, tokenizes subword.
 */
static t_tok	*_match_word_or_name_or_exception(t_state *s, t_lex *lexer)
{
	dvprint(_MOD_ ": ---- %s: got %s\n", __FUNCTION__, lexer->buf);
	if (true == is_normal_delim(lexer, 0) && ft_strlen(lexer->buf) > 0)
	{
		return (lex_create_token(get_mem(s), lexer, word_or_name(lexer->buf)));
	}
	if (true == is_transition_delim(lexer) && ft_strlen(lexer->buf) > 0)
	{
		lexer->is_subtoken = true;
		return (lex_create_token(get_mem(s), lexer, word_or_name(lexer->buf)));
	}
	return (NULL);
}

/*
 * Advances on input until delim or transition char.
 * Leaves tokenization for downstream unless word found in ht.
 */
static t_tok	*_match_reserved_token(t_state *s, t_lex *l)
{
	t_tok	*res;

	res = NULL;
	dvprint(_MOD_ ": ---- %s\n", __FUNCTION__);
	dvprint(_MOD_ ": ------ Ptr: '%c'\n", *l->ptr);
	while (*l->ptr && false == is_normal_delim(l, 0)
		&& false == is_transition_delim(l))
	{
		process_escape_sequence(l);
		if (SKIPREST == process_special_operators(l))
			continue ;
		l->buf[(l->buf_idx)++] = *l->ptr++;
		if (ft_strnlen(l->buf, DEBUG_BUF_SIZE) == DEBUG_BUF_SIZE)
			dvprint("(TOO MUCH TO PRINT)\n");
		else
			dvprint(_MOD_ ": ------ Buf: '%s' idx:%d_\n", l->buf, l->buf_idx);
		if (ft_strnlen(l->buf, LEX_BUFSZ - 2) == LEX_BUFSZ - 2)
			return (lex_set_err(l), NULL);
		res = lex_ht_lookup(s, l);
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

	dvprint(_MOD_ ": ---- %s\n", __FUNCTION__);
	res = NULL;
	while (' ' == *lexer->ptr || '\t' == *lexer->ptr)
		lexer->ptr++;
	if (true == is_transition_delim(lexer))
		return (NULL);
	res = _match_reserved_token(s, lexer);
	if (res)
		return (dvprint(_MOD_ ": ---- Done, reserved tok. \n"), res);
	res = _match_word_or_name_or_exception(s, lexer);
	if (res)
		return (dvprint(_MOD_ ": ---- Done, word tok. \n"), res);
	if ((unsigned char)OP_NULL == *lexer->ptr)
		return (dvprint(_MOD_ ": ----FOUND NULL\n"), NULL);
	res = _match_normal_op(s, lexer);
	if (res)
		return (dvprint(_MOD_ ": ---- Done, op tok. \n"), res);
	return (NULL);
}

/* Expected to add multiple tokens to the llist
 * Adds token to llist when ptr advances to a
 * delimiter char or transition char. Tokenizes transition operator.
 * If starting on a normal delim, commits any incomplete grp token.
 */
int	tokenize_normal(t_state *s, t_lex *lexer)
{
	t_tok	*token;

	dprint(_MOD_ YELLOW ": STATE: %s\n" RESET, __FUNCTION__);
	while (lexer->ptr && !is_transition_delim(lexer))
	{
		if (is_normal_delim(lexer, 0) && lexer->last_grp_tok)
			add_grptoken(get_mem(s), lexer);
		token = _match_normal(s, lexer);
		if (lexer->lex_err)
			return (pbufflow(NULL), ERR_GENERAL);
		if (lexer->ptr)
			dprint(_MOD_ ": ptr at:_%c_\n", *lexer->ptr);
		if (token)
			if (0 != add_token(get_mem(s), lexer, token))
				return (ERR_GENERAL);
		if (OP_NULL == *lexer->ptr)
		{
			dvprint(_MOD_ ": ##### TOK EOF FOUND\n");
			return (0);
		}
	}
	return (0);
}
