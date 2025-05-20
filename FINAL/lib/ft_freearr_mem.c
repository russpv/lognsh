#include "../include/libft.h"

/* Frees arrays with optional size argument */
void	ft_freearr(void **arr, int size)
{
	char	**ptr;
	int		i;

	i = -1;
	if (NULL == arr)
		return ;
	ptr = (char **)arr;
	if (size > 0)
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

void	ft_freearr_mem(t_mem_node *head, t_dealloc_fn dealloc, \
void **arr, int size)
{
	char	**ptr;
	int		i;

	i = -1;
	if (NULL == arr)
		return ;
	ptr = (char **)arr;
	if (size > 0)
	{
		while (++i < size)
			if (ptr[i])
				dealloc(head, ptr[i]);
	}
	else
	{
		if (*ptr)
		{
			while (*ptr)
			{
				dealloc(head, *ptr);
				ptr++;
			}
		}
	}
	dealloc(head, arr);
}
