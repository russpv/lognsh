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
#include "../lib/libft_function_utils.h"
/* SPLIT
** Returns new C-strings split by c or NULL (malloc)
** Array is null terminated
** Allows empty strings from consecutive c's
*/
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
/*char	**ft_split_ifs_mem(struct s_mem_utils *m, \
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
}*/
static char	**_fill_arr(struct s_mem_utils *m, char **arr, \
			char *s2, char const *set)
{
	char			*temp;
	unsigned int	i;

	i = 0;
	while (s2)
	{
		if (!*s2)
			break ;
		temp = _get_temp_if(s2, set);
		if (temp - s2 > 0)
		{
			if (!_copy_word(m, s2, &(arr[i]), temp - s2))
			{
				_arr_free(m, arr, i);
				return (NULL);
			}
			i++;
		}
		s2 = temp + 1;
	}
	return (arr);
}

char	**ft_split_ifs_mem(struct s_mem_utils *m, \
		char const *s, char const *set)
{
	char	**arr;
	size_t	count;
	char	*s2;

	s2 = ft_skip_delims(s, set);
	count = _get_word_count_if(s2, set);
	arr = _init_arr(m, count);
	if (!arr)
		return (NULL);
	arr = _fill_arr(m, arr, s2, set);
	if (!arr)
		return (NULL);
	return (arr);
}
