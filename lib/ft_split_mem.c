#include "../include/libft.h"


/* SPLIT
** Returns new C-strings split by c or NULL (malloc)
** Array is null terminated
** Allows empty strings from consecutive c's
*/

static inline void	_arr_free(struct s_mem_utils *m, char **arr, unsigned int i)
{
	unsigned int	j;

	j = 0;
	while (j < i)
		m->u(m->head, arr[j++]);
	m->u(m->head, arr);
}

static inline t_bool	_is_print(char const *p)
{
	while (*p)
		if (ft_isprint(*p++))
			return (TRUE);
	return (FALSE);
}

/* Returns non-empty string count in C-string p*/
static inline int	_get_word_count(char const *p, char ch)
{
	const char		*temp;
	int				count;
	unsigned char	c;

	count = 0;
	c = (unsigned char)ch;
	if (ch == 0 && _is_print(p))
		return (1);
	while (*p)
	{
		temp = ft_strchr(p, c);
		if (!temp)
		{
			if (_is_print(p))
				count++;
			break ;
		}
		if (temp - p > 0)
			count++;
		p = temp + 1;
	}
	return (count);
}

static inline void	*_copy_word(struct s_mem_utils *m, char const *s, char **elem, size_t len)
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
	char *temp;

	temp = ft_strchr(str, delim);
	if (!temp)
		temp = ft_strchr(str, 0);
	return (temp);
}

static char **_init_arr(struct s_mem_utils *m, size_t count)
{
	char **arr;

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
	char			**arr;
	char const		*temp;
	size_t				count;
	unsigned int	i;

	count = _get_word_count(s, c);
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
				return (_arr_free(m, arr, i), NULL);
			--count;
		}
		s = temp + 1;
	}
	return (arr);
}
