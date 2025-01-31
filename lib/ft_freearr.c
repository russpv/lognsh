#include "../include/libft.h"

/* Frees arrays with optional size argument */
void	ft_freearr(void **arr, int size)
{
	char	**ptr;
	int		i;

	i = -1;
	ptr = (char **)arr;
	if (size)
	{
		while (++i < size)
			if (ptr[i])
				free(ptr[i]);
	}
	else
	{
		if (*ptr)
		{
			while (*ptr)
			{
				free(*ptr);
				ptr++;
			}
		}
	}
	free(arr);
}
