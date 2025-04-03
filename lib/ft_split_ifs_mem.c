/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_split_ifs_mem.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dayeo <dayeo@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/03 15:50:32 by dayeo             #+#    #+#             */
/*   Updated: 2025/04/03 15:50:32 by dayeo            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
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
static inline int	_get_word_count(char const *p, char const *set)
{
	const char		*temp;
	int				count;

	count = 0;
	while (*p)
	{
		temp = ft_strchrs(p, set);
		if (!temp)
		{
			if (_is_print(p))
				count++;
			break ;
		}
		else if (temp - p > 0)
			count++;
		p = temp + 1;
	}
	return (count);
}

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

static char	*_get_temp(char const *str, char const *delims)
{
	char	*temp;

	temp = ft_strchrs(str, delims);
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

/* SPLIT IFS
** Returns new C-strings split by string set (cannot have \0)
** Array is null terminated
** Skips initial whitespace
*/
char	**ft_split_ifs_mem(struct s_mem_utils *m, \
		char const *s, char const *set)
{
	char			**arr;
	char			*temp;
	size_t			count;
	unsigned int	i;
	char			*s2;

	s2 = ft_skip_delims(s, set);
	count = _get_word_count(s2, set);
	arr = _init_arr(m, count);
	if (!arr)
		return (NULL);
	i = 0;
	while (s2 && count)
	{
		if (!*s2)
			break ;
		temp = _get_temp(s2, set);
		if (temp - s2 > 0)
		{
			if (!_copy_word(m, s2, &(arr[i++]), temp - s2))
				return (_arr_free(m, arr, i), NULL);
			--count;
		}
		s2 = temp + 1;
	}
	return (arr);
}
