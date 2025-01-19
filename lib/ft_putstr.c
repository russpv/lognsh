#include "libft.h"

void	ft_putstr(const char *c)
{
	while (*c)
		write(1, c++, 1);
}
