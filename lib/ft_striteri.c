#include "libft.h"

/* STRITERI
** Returns modified string with func f applied to each char
*/

void	ft_striteri(char *s, void (*f)(unsigned int, char *))
{
	unsigned int	i;

	i = 0;
	while (s[i])
	{
		f(i, s + i);
		i++;
	}
	return ;
}
