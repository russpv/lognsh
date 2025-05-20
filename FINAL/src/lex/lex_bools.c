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
