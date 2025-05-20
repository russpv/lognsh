#include "lex_int.h"

bool	is_varnamechar(unsigned char c)
{
	dvprint(_MOD_ ": %s: (%c)\n", __FUNCTION__, c);
	return (ft_isalnum((int)c) || '_' == c);
}

// Checks for any special substition char
bool	is_specialchar(unsigned char c)
{
	dvprint(_MOD_ ": %s: (%c)\n", __FUNCTION__, c);
	return ('?' == c);
}

bool	is_too_long(t_lex *l, const char *input)
{
	if (NULL == input)
		return (true);
	if (ft_strnlen(input, LEX_BUFSZ) >= \
LEX_BUFSZ - ft_strnlen(l->buf, LEX_BUFSZ))
	{
		dprint(_MOD_ ": ERROR: Input exceeds buf size.\n");
		print_lex_buffer_overflow();
		return (true);
	}
	return (false);
}


/* -------------------------------------------------------------------------
 * Function Group: Lexer delimiter tests
 * Description   : Handles logic for determining token delimeters
 * ------------------------------------------------------------------------- */

// Checks for '&&'
bool	is_logicalop(unsigned char c, unsigned char next)
{
	if ('&' == c)
	{
		if (next)
			if ('&' == next)
				return (true);
	}
	return (false);
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
	dvprint(_MOD_ ": -------- %s:_%c_", __FUNCTION__, c);
	if (!*lexer->ptr)
		return (dvprint(" YES-NULL\n"), true);
	if (ft_strchr(NORMALTRANSITIONS, c))
	{
		if (c == '<')
		{
			if (next)
				if ('<' == next)
					return (dvprint(" YES, <<\n"), true);
		}
		else if (is_dollar_delim(lexer))
			return (dvprint(" YES, $_\n"), true);
		else if ('$' != c)
			return (dvprint(" YES\n"), true);
	}
	dvprint(" NO\n");
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
		dvprint(_MOD_ ": -------- %s:_%c%c_", __FUNCTION__, c, next);
	else if (c)
		dvprint(_MOD_ ": -------- %s:_%c__", __FUNCTION__, c);
	if (ft_strchr(NORMALDELIMS, c))
	{
		if (true == is_logicalop(c, next))
			return (dvprint(" YES\n"), true);
		else if (c == '&')
			return (dvprint(" NO\n"), false);
		return (dvprint(" YES\n"), true);
	}
	return (dvprint(" NO\n"), false);
}
