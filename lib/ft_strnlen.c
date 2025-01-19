#include "libft.h"

/* STRNLEN
** Returns # of bytes in s excl NULL, but at most maxlen 
*/

size_t	ft_strnlen(const char *s, size_t maxlen)
{
	size_t	i;

	i = 0;
	while (s[i] && i < maxlen)
		i++;
	return (i);
}
