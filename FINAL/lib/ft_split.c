#include "../include/libft.h"
/* SPLIT
** Returns new C-strings split by c or NULL (malloc)
** Array is null terminated
** Allows empty strings from consecutive c's
*/
static inline void	arr_free(char **arr, unsigned int i)
{
	unsigned int	j;

	j = 0;
	while (j < i)
		free(arr[j++]);
	free(arr);
}

static inline t_bool	is_print(char const *p)
{
	while (*p)
		if (ft_isprint(*p++))
			return (TRUE);
	return (FALSE);
}

/* Returns non-empty string count in C-string p*/
static inline int	get_word_count(char const *p, char ch)
{
	const char		*temp;
	int				count;
	unsigned char	c;

	count = 0;
	c = (unsigned char)ch;
	if (ch == 0 && is_print(p))
		return (1);
	while (*p)
	{
		temp = ft_strchr(p, c);
		if (!temp)
		{
			if (is_print(p))
				count++;
			break ;
		}
		if (temp - p > 0)
			count++;
		p = temp + 1;
	}
	return (count);
}

static inline void	*copy_word(char const *s, char ***arr, char const *temp,
		unsigned int i)
{
	size_t	len;

	len = temp - s;
	(*arr)[i] = malloc(sizeof(char) * (len + 1));
	if (!(*arr)[i])
	{
		arr_free((*arr), i);
		return (NULL);
	}
	(*arr)[i][len] = 0;
	ft_memmove((*arr)[i], s, len);
	return ((*arr)[i]);
}

char	**ft_split(char const *s, char c)
{
	char			**arr;
	char const		*temp;
	int				count;
	unsigned int	i;

	count = get_word_count(s, c);
	arr = malloc(sizeof(char *) * (count + 1));
	if (!arr)
		return (NULL);
	arr[count] = NULL;
	i = 0;
	while (*s && count)
	{
		temp = ft_strchr(s, c);
		if (!temp)
			temp = ft_strchr(s, 0);
		if (temp - s > 0)
		{
			if (!copy_word(s, &arr, temp, i++))
				return (NULL);
			--count;
		}
		s = temp + 1;
	}
	return (arr);
}
