#include "../include/libft.h"

char	*ft_skip_delims(char const *s, char const *ref)
{
	char	*delims;
	char	*word;

	if (!s || !ref)
		return (NULL);
	word = (char *)s;
	while (*word)
	{
		delims = (char *)ref;
		while (*delims)
		{
			if (*word == *delims)
				break ;
			delims++;
		}
		if (0 == *delims)
			return (word);
		word++;
	}
	return (NULL);
}
