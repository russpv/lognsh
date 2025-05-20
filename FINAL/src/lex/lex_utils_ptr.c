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
