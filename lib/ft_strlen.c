#include "../include/libft.h"

/* STRLEN
** This returns length of the string pointed to by 'c'
** NOT including '\0' (e.g. 'size')
** Will NOT SEGV if c is NULL (UNlike strlen())
*/
size_t	ft_strlen(const char *c)
{
	size_t	i;

	i = 0;
	if (NULL == c)
		return (0);
	while (c[i])
		i++;
	return (i);
}
