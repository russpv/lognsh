#include "llist_int.h"

void	get_root(char *buf, void *s)
{
	char	*str;
	size_t	buf_idx;

	str = (char *)s;
	buf_idx = 0;
	while (*str && buf_idx < MAX_BUF)
	{
		if (ft_isalnum(*str))
			buf[buf_idx++] = *str;
		str++;
	}
}
