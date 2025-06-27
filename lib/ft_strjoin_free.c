#include "../include/libft.h"

char	*ft_strjoin_free(char *s1, const char *s2)
{
	char	*res;

	if (!s1 || !s2)
		return (NULL);
	res = ft_strjoin(s1, s2);
	if (!res)
	{
		free(s1);
		return (NULL);
	}
	free(s1);
	return (res);
}
