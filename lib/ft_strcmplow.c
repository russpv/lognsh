#include "../include/libft.h"
#include <stdio.h>

int	ft_strcmplow(const char *s1, const char *s2)
{
	size_t	i;

	i = 0;
	int res;
	while (s1[i] || s2[i])
	{
		const unsigned char l = ft_tolower((unsigned char)s1[i]);
		const unsigned char r = ft_tolower((unsigned char)s2[i]);
		if (l != r)
		{
			res = l - r;
			return (res);
		}
		i++;
	}
	return (0);
}
