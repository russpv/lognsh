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
	getchar();
	return (lexer->buf[0] == '$' && *(lexer->ptr) == '?');
}

// returns TOK_WORD if alphanumeric or underscore, else TOK_NAME
int	word_or_name(const char *s)
{
	dprint(_MOD_ ": -------- %s:_%s_", __FUNCTION__, s);
	if (!*s)
	{
		dprint(" ERR\n");
		return (TOK_ERR);
	}
	while (*s)
	{
		if (!(ft_isalnum(*s) || '_' == *s))
		{
			dprint(" WORD\n");
			return (TOK_WORD);
		}
		s++;
	}
	dprint(" NAME\n");
	return (TOK_NAME);
}
