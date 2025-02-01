#include "../include/libft.h"


/* STRNCPY
** This copies src into dest up to n places; assumes space for null
** inefficient in that it writes to every dest byte
** see memcpy
*/

char	*ft_strncpy(char *dest, const char *src, size_t n)
{
	unsigned int	i;

	i = 0;
	while (src[i] && i < n)
	{
		dest[i] = src[i];
		i++;
	}
	while (i < n)
	{
		dest[i] = '\0';
		i++;
	}
	return (dest);
}
