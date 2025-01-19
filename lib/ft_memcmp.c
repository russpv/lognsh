#include "libft.h"

/* MEMCMP
** Compares n bytes between mem regions
** Returns zero if match
** <0 if lhs is less than rhs
** >0 if lhs is greater than rhs
*/

int	ft_memcmp(const void *p, const void *q, size_t n)
{
	const unsigned char	*a = (const unsigned char *)p;
	const unsigned char	*b = (const unsigned char *)q;

	while (n-- && *a++ - *b++ == 0)
		;
	return (*(a - 1) - *(b - 1));
}
