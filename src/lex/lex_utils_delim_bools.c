#include "lex_int.h"

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
