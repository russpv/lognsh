/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_split_mem_utils.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dayeo <dayeo@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/04 13:04:40 by dayeo             #+#    #+#             */
/*   Updated: 2025/04/04 13:04:46 by dayeo            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft_function_utils.h"

void	_arr_free(struct s_mem_utils *m, char **arr, unsigned int i)
{
	unsigned int	j;

	j = 0;
	while (j < i)
		m->u(m->head, arr[j++]);
	m->u(m->head, arr);
}

t_bool	_is_print(char const *p)
{
	while (*p)
		if (ft_isprint(*p++))
			return (TRUE);
	return (FALSE);
}

/* Returns non-empty string count in C-string p*/
int	_get_word_count(char const *p, char ch)
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

void	*_copy_word(struct s_mem_utils *m, \
				char const *s, char **elem, size_t len)
{
	*elem = m->f(m->head, sizeof(char) * (len + 1));
	if (!*elem)
		return (NULL);
	(*elem)[len] = 0;
	ft_memmove(*elem, s, len);
	return (*elem);
}

char	*_get_temp(char const *str, char delim)
{
	char	*temp;

	temp = ft_strchr(str, delim);
	if (!temp)
		temp = ft_strchr(str, 0);
	return (temp);
}
