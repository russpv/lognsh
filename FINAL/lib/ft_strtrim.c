#include "../include/libft.h"

/* STRTRIM
** Returns a new c-string copy of s1 without chars in set
** at front and back of s1
** least sig bit higher address
** UNPROTECTED
*/
int	ft_is_set(char const *s1, char const *set)
{
	while (*s1 && *set)
		if (*s1 == *set++)
			return (1);
	return (0);
}

char	*ft_strtrim(char const *s1, char const *set)
{
	size_t	len;
	char	*str;
	char	*end;

	str = (char *)s1;
	while (ft_is_set(s1, set))
		s1++;
	end = ft_strchr(s1, 0);
	if (end != s1)
	{
		while (ft_is_set(--end, set))
			;
		len = end - s1 + 1;
	}
	else
		len = 0;
	str = malloc(sizeof(char) * len + 1);
	if (!str)
		return (NULL);
	str[len] = 0;
	return (ft_memcpy(str, s1, len));
}

char	*ft_strtrim_mem(struct s_mem_utils *m, char const *s1, char const *set)
{
	size_t	len;
	char	*str;
	char	*end;

	str = (char *)s1;
	while (ft_is_set(s1, set))
		s1++;
	end = ft_strchr(s1, 0);
	if (end != s1)
	{
		while (ft_is_set(--end, set))
			;
		len = end - s1 + 1;
	}
	else
		len = 0;
	str = m->f(m->head, sizeof(char) * len + 1);
	if (!str)
		return (NULL);
	str[len] = 0;
	return (ft_memcpy(str, s1, len));
}

char	*ft_strtrimfront_mem(struct s_mem_utils *m, \
char const *s1, char const *set)
{
	size_t	len;
	char	*str;
	char	*end;

	str = (char *)s1;
	while (ft_is_set(s1, set))
		s1++;
	end = ft_strchr(s1, 0);
	if (end != s1)
	{
		len = end - s1 + 1;
	}
	else
		len = 0;
	str = m->f(m->head, sizeof(char) * len + 1);
	if (!str)
		return (NULL);
	str[len] = 0;
	return (ft_memcpy(str, s1, len));
}

char	*ft_strtrimback_mem(struct s_mem_utils *m, \
char const *s1, char const *set)
{
	size_t	len;
	char	*str;
	char	*end;

	str = (char *)s1;
	end = ft_strchr(s1, 0);
	if (end != s1)
	{
		while (ft_is_set(--end, set))
			;
		len = end - s1 + 1;
	}
	else
		len = 0;
	str = m->f(m->head, sizeof(char) * len + 1);
	if (!str)
		return (NULL);
	str[len] = 0;
	return (ft_memcpy(str, s1, len));
}
