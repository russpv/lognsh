/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_split_utils.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dayeo <dayeo@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/04 11:51:36 by dayeo             #+#    #+#             */
/*   Updated: 2025/04/04 11:52:21 by dayeo            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft_function_utils.h"

void	_arr_free_if(struct s_mem_utils *m, char **arr, unsigned int i)
{
	unsigned int	j;

	j = 0;
	while (j < i)
		m->u(m->head, arr[j++]);
	m->u(m->head, arr);
}

t_bool	_is_print_if(char const *p)
{
	while (*p)
		if (ft_isprint(*p++))
			return (TRUE);
	return (FALSE);
}

/* Returns non-empty string count in C-string p*/
int	_get_word_count_if(char const *p, char const *set)
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

void	*_copy_word_if(struct s_mem_utils *m, \
				char const *s, char **elem, size_t len)
{
	*elem = m->f(m->head, sizeof(char) * (len + 1));
	if (!*elem)
		return (NULL);
	(*elem)[len] = 0;
	ft_memmove(*elem, s, len);
	return (*elem);
}

char	*_get_temp_if(char const *str, char const *delims)
{
	char	*temp;

	temp = ft_strchrs(str, delims);
	if (!temp)
		temp = ft_strchr(str, 0);
	return (temp);
}
