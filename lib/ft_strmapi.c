#include "../include/libft.h"


/* STRMAPI
** Returns new string by applying func f to each char in s
** or NULL if bad malloc
** arg1: index of char
** arg2: char literal
*/

char	*ft_strmapi(char const *s, char (*f)(unsigned int, char))
{
	char			*res;
	unsigned int	i;

	res = malloc(sizeof(char) * (ft_strlen(s) + 1));
	if (!res)
		return (NULL);
	res[ft_strlen(s)] = 0;
	i = 0;
	while (s[i])
	{
		res[i] = f(i, s[i]);
		i++;
	}
	return (res);
}
