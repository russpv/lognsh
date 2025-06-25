#include "../include/libft.h"

char	*ft_strndup(const char *s, size_t size)
{
	char	*dup;
	size_t	i;

	if (!s)
		return (NULL);
	dup = malloc(size + 1);
	if (!dup)
		return (NULL);
	i = 0;
	while (i < size && s[i])
	{
		dup[i] = s[i];
		i++;
	}
	dup[i] = '\0';
	return (dup);
}
