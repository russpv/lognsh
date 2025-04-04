/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_split_mem.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dayeo <dayeo@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/03 15:50:39 by dayeo             #+#    #+#             */
/*   Updated: 2025/04/03 16:34:20 by dayeo            ###   ########.fr       */
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
		if (*temp == '\0')
			break ;
	}
	return (arr);
}
