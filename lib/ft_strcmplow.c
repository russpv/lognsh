#include "../include/libft.h"

int	ft_strcmp_low(const char *s1, const char *s2)
{
	size_t				i;
	int					res;
	unsigned char		l;
	unsigned char		r;

	i = 0;
	while (s1[i] || s2[i])
	{
		l = ft_tolower((unsigned char)s1[i]);
		r = ft_tolower((unsigned char)s2[i]);
		if (l != r)
		{
			res = l - r;
			return (res);
		}
		i++;
	}
	return (0);
}
