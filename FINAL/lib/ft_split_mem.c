#include "../include/libft.h"
/* SPLIT
** Returns new C-strings split by c or NULL (malloc)
** Array is null terminated
** Allows empty strings from consecutive c's
*/

static inline void	*_copy_word(struct s_mem_utils *m, \
char const *s, char **elem, size_t len)
{
	*elem = m->f(m->head, sizeof(char) * (len + 1));
	if (!*elem)
		return (NULL);
	(*elem)[len] = 0;
	ft_memmove(*elem, s, len);
	return (*elem);
}

static char	*_get_temp(char const *str, char delim)
{
	char	*temp;

	temp = ft_strchr(str, delim);
	if (!temp)
		temp = ft_strchr(str, 0);
	return (temp);
}

static char	**_init_arr(struct s_mem_utils *m, size_t count)
{
	char	**arr;

	arr = m->f(m->head, sizeof(char *) * (count + 1));
	if (!arr)
		return (NULL);
	arr[count] = NULL;
	return (arr);
}

/* SPLIT
** Returns new C-strings split by c or NULL (malloc)
** Array is null terminated
** Allows empty strings from consecutive c's
*/
char	**ft_split_mem(struct s_mem_utils *m, char const *s, char c)
{
	size_t			count;
	char			**arr;
	char const		*temp;
	unsigned int	i;

	count = get_word_count_split(s, c);
	arr = _init_arr(m, count);
	if (!arr)
		return (NULL);
	i = 0;
	while (*s && count)
	{
		temp = _get_temp(s, c);
		if (temp - s > 0)
		{
			if (!_copy_word(m, s, &(arr[i++]), temp - s))
				return (arr_free_split(m, arr, i), NULL);
			--count;
		}
		s = temp + 1;
		if (*temp == '\0')
			break ;
	}
	return (arr);
}
