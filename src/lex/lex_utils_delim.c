#include "lex_int.h"

// Returns next char, if valid
unsigned char	is_valid_next(t_lex *l, int offset)
{
	if (!l || !l->ptr)
		return (0);
	if (offset < 0 || (size_t)(offset + 1) >= ft_strlen(l->ptr) || l->ptr[offset
		+ 1] == '\0')
		return (0);
	return (l->ptr[offset + 1]);
}

// Returns prev char, if valid
unsigned char	is_valid_prev(t_lex *l, int offset)
{
	char	*ptr;

	(void)offset;
	if (!l || !l->ptr)
		return (0);
	ptr = l->ptr;
	ptr--;
	if (NULL == ptr)
		return (0);
	if (0 == *ptr)
		return (0);
	return (*(l->ptr - 1));
}

// Tests for beginning of $ commands
// '$' followed by correct char (varname or ?)
bool	is_dollar_delim(t_lex *l)
{
	if (!l || !l->ptr || !l->ptr[0] || !l->ptr[1])
		return (false);
	return ('$' == l->ptr[0] && (is_varnamechar(l->ptr[1])
			|| is_specialchar(l->ptr[1])));
}

/* A subset of normal delimiters to trigger state
 * transition. If "<<" , must change state
 */
bool	is_transition_delim(t_lex *lexer)
{
	unsigned char	c;
	unsigned char	next;

	if ('\0' == lexer->ptr[0])
		return (true);
	c = lexer->ptr[0];
	next = is_valid_next(lexer, 0);
	debug_print(_MOD_ ": -------- %s:_%c_", __FUNCTION__, c);
	if (!*lexer->ptr)
		return (debug_print(" YES-NULL\n"), true);
	if (ft_strchr(NORMALTRANSITIONS, c))
	{
		if (c == '<')
		{
			if (next)
				if ('<' == next)
					return (debug_print(" YES, <<\n"), true);
		}
		else if (is_dollar_delim(lexer))
			return (debug_print(" YES, $_\n"), true);
		else if ('$' != c)
			return (debug_print(" YES\n"), true);
	}
	debug_print(" NO\n");
	return (false);
}

static bool	_is_valid_offset(t_lex *l, int offset)
{
	if (!l || !l->ptr)
		return (false);
	if (offset < 0 || l->ptr[offset] == '\0')
		return (false);
	return (true);
}

// True if (2-)char sequence delimits a token
// Some delimeters are 1-char only (NOTDELIMITED)
// Looks ahead for any doubles, '&&'
// Returns true if ptr on \0 EOF
bool	is_normal_delim(t_lex *lexer, int offset)
{
	unsigned char	c;
	unsigned char	next;

	if ('\0' == *lexer->ptr)
		return (true);
	if (false == _is_valid_offset(lexer, offset))
		return (true);
	c = *lexer->ptr + offset;
	next = is_valid_next(lexer, offset);
	if (c && next)
		debug_print(_MOD_ ": -------- %s:_%c%c_", __FUNCTION__, c, next);
	else if (c)
		debug_print(_MOD_ ": -------- %s:_%c__", __FUNCTION__, c);
	if (ft_strchr(NORMALDELIMS, c))
	{
		if (true == is_logicalop(c, next))
			return (debug_print(" YES\n"), true);
		else if (c == '&')
	 		return (debug_print(" NO\n"), false);
		return (debug_print(" YES\n"), true);
	}
	return (debug_print(" NO\n"), false);
}
