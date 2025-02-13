#include "lex_int.h"

/* When '$' in the buf and '?' is ptr */
bool	is_dollar_question(t_lex *lexer)
{
	return (lexer->buf[0] == '$' && *(lexer->ptr) == '?');
}

bool	is_normal_delim(unsigned char s)
{
	debug_print("Lexer: --------_is_normal_delim:_%c_", s);
	if (ft_strchr(NORMALDELIMS, s))
	{
		debug_print("Lexer: YES\n");
		return (true);
	}
	debug_print("Lexer: NO\n");
	return (false);
}

/* A subset of normal delimiters to trigger state
 * transition. If "<<" , must change state
 */
bool	is_transition_char(t_lex *l, unsigned char s)
{
	debug_print("Lexer: --------_is_transition_char:_%c_", s);
	if (s == 0)
	{
		debug_print("Lexer: YES-NULL\n");
		return (true);
	}
	if (ft_strchr(NORMALTRANSITIONS, s))
	{
		if (s == '<')
		{
			if ('<' == *(l->ptr + 1))
			{
				debug_print("Lexer: YES\n");
				return (true);
			}
		}
		else
		{
			debug_print("Lexer: YES\n");
			return (true);
		}
	}
	debug_print("Lexer: NO\n");
	return (false);
}

int	word_or_name(const char *s)
{
	debug_print("Lexer: --------_word_or_name:_%s_", s);
	if (!*s)
	{
		debug_print("Lexer: ERR\n");
		return (TOK_ERR);
	}
	while (*s)
	{
		if (!(ft_isalnum(*s) || '_' == *s))
		{
			debug_print("Lexer: WORD\n");
			return (TOK_WORD);
		}
		s++;
	}
	debug_print("Lexer: NAME\n");
	return (TOK_NAME);
}
