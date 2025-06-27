#include "../include/libft.h"

void	arr_free_split(struct s_mem_utils *m, char **arr, unsigned int i)
{
	unsigned int	j;

	j = 0;
	while (j < i)
		m->u(m->head, arr[j++]);
	m->u(m->head, arr);
}

t_bool	is_print_split(char const *p)
{
	while (*p)
		if (ft_isprint(*p++))
			return (TRUE);
	return (FALSE);
}

/* Returns non-empty string count in C-string p*/
int	get_word_count_split(char const *p, char ch)
{
	const char		*temp;
	int				count;
	unsigned char	c;

	count = 0;
	c = (unsigned char)ch;
	if (ch == 0 && is_print_split(p))
		return (1);
	while (*p)
	{
		temp = ft_strchr(p, c);
		if (!temp)
		{
			if (is_print_split(p))
				count++;
			break ;
		}
		if (temp - p > 0)
			count++;
		p = temp + 1;
	}
	return (count);
}
