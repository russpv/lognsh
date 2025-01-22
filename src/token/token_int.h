#ifndef TOKEN_INT_H
# define TOKEN_INT_H

#include "token.h"

struct s_tok
{
	enum e_tok_type	type;
	char			*raw;
	size_t			pos;
	bool			do_globbing;
	bool			do_expansion;
};

#endif