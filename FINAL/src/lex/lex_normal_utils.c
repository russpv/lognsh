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
