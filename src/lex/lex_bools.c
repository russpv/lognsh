
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
