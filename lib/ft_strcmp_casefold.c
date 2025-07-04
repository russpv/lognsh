#include "../include/libft.h"

/* Places upper case after lower case */
int	ft_strcmp_casefold(const char *s1, const char *s2)
{
	size_t				i;
	unsigned char		l;
	unsigned char		r;

	i = 0;
	while (s1[i] || s2[i])
	{
		l = ft_tolower((unsigned char)s1[i]);
		r = ft_tolower((unsigned char)s2[i]);
		if (l != r)
			return (l - r);
		i++;
	}
	i = 0;
	while (s1[i] || s2[i])
	{
		l = ((unsigned char)s1[i]);
		r = ((unsigned char)s2[i]);
		if (l != r)
			return (-(l - r));
		i++;
	}
	return (0);
}
