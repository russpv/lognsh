#include "../include/libft.h"

/* STRCPY 
** This copies src into dest, w/ null, and returns pointer
** It assumes the correct lengths have been passed 
*/
char	*ft_strcpy(char *dest, const char *src)
{
	unsigned int	i;

	i = 0;
	while (src[i])
	{
		dest[i] = src[i];
		i++;
	}
	dest[i] = '\0';
	return (dest);
}
