#include "lex_int.h"

/* When '$' in the buf and '?' is ptr */
bool is_dollar_question(t_lex *lexer)
{
    return (lexer->buf[0] == '$' && *(lexer->ptr) == '?');
}

bool	is_normal_delim(unsigned char s)
{
	debug_print("--------_is_normal_delim:_%c_", s);
	if (ft_strchr(NORMALDELIMS, s))
	{
		debug_print("YES\n");
		return (true);
	}
	debug_print("NO\n");
	return (false);
}

/* A subset of normal delimiters to trigger state
 * transition. If "<<" , must change state
 */
bool	is_transition_char(t_lex *l, unsigned char s)
{
	debug_print("--------_is_transition_char:_%c_", s);
	if (s == 0)
	{
		debug_print("YES-NULL\n");
		return (true);
	}
	if (ft_strchr(NORMALTRANSITIONS, s))
	{
		if (s == '<')
		{
			if ('<' == *(l->ptr + 1))
			{
				debug_print("YES\n");
				return (true);
			}
		}
		else
		{
			debug_print("YES\n");
			return (true);
		}
	}
	debug_print("NO\n");
	return (false);
}

int	word_or_name(const char *s)
{
	debug_print("--------_word_or_name:_%s_", s);
	if (!*s)
	{
		debug_print("ERR\n");
		return (TOK_ERR);
	}
	while (*s)
	{
		if (!(ft_isalnum(*s) || '_' == *s))
		{
			debug_print("WORD\n");
			return (TOK_WORD);
		}
		s++;
	}
	debug_print("NAME\n");
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
// Advance the lexer pointer past comments
int	process_special_operators(t_lex *lexer)
{
	if ((unsigned char)OP_ENV == *lexer->ptr && false == lexer->escape_mode)
		lexer->do_expansion = true;
	if ((unsigned char)OP_STAR == *lexer->ptr && false == lexer->escape_mode)
		lexer->do_globbing = true;
	if ((unsigned char)OP_HASH == *lexer->ptr && false == lexer->escape_mode)
	{
		lexer->escape_mode = false;
		while (*lexer->ptr && *lexer->ptr != TK_NEWL && *lexer->ptr != TK_EOF)
			lexer->ptr++;
		return (1);
	}
	lexer->escape_mode = false;
	return (0);
}
