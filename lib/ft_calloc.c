#include "../include/libft.h"

/* CALLOC
** Returns ptr to heap memory for an array of num obj
** of size, and inits to zero.
** If size is zero, returns NULL
*/

void	*ft_calloc(size_t num, size_t size)
{
	void	*p;

	p = malloc(size * num);
	if (!p)
		return (NULL);
	ft_bzero(p, size * num);
	return (p);
}
