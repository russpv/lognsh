#include "../include/libft.h"
#include <stdio.h>

/* STRCHR
** Returns ptr to first occurence of char
** or NULL
** UNPROTECTED
** Note: gcc runs it faster w/o longword checking or independent
** loop instrucs
*/
char	*ft_strchr(const char *s, int c_in)
{
	const unsigned char	c = c_in;
	char				*p;

	p = (char *)s;
	while (1)
	{
		if (*p == c)
			return (p);
		else if (!*p++)
			return (NULL);
		if (*p == c)
			return (p);
		else if (!*p++)
			return (NULL);
		if (*p == c)
			return (p);
		else if (!*p++)
			return (NULL);
		if (*p == c)
			return (p);
		else if (!*p++)
			return (NULL);
	}
}

/* STRCHRS
** Returns ptr to first occurrence of any char in char set
** or NULL
** UNPROTECTED
** Note: gcc runs it faster w/o longword checking or independent
** loop instrucs
*/
char	*ft_strchrs(const char *raw, const char *chars)
{
	char		c;
	const char	*res;
	char		*nearest;

	if (!raw || !chars)
		return (NULL);
	nearest = NULL;
	while (*chars)
	{
		c = *chars;
		res = (const char *)ft_strchr(raw, c);
		if (res && (NULL == nearest || res < nearest))
			nearest = (char *)res;
		chars++;
	}
	return (nearest);
}

char	*ft_strchrs_betw(const char *start, const char *end, const char *chars)
{
	char		c;
	const char	*res;
	char		*nearest;

	if (!start || !end || !chars)
		return (NULL);
	nearest = NULL;
	while (*chars)
	{
		c = *chars;
		res = (const char *)ft_strchr(start, c);
		if (res <= end)
			if (res && (NULL == nearest || res < nearest))
				nearest = (char *)res;
		chars++;
	}
	return (nearest);
}
