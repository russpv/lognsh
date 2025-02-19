#include "../include/libft.h"

int	ft_strcmplow(const char *s1, const char *s2)
{
	size_t	i;

	i = 0;
	int res;
	while (s1[i] || s2[i])
	{
		if (s1[i] != s2[i])
		{
			res = ft_tolower((unsigned char)s1[i]) \
			- ft_tolower((unsigned char)s2[i]);
			return (res);
		}
		i++;
	}
	return (0);
}
