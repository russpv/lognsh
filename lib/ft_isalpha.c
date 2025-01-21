#include "../include/libft.h"

/* ISALPHA
** Tests if char is alphabetic
** behavior is undefined if not passed as unsigned char
** but we return 0 here
*/

t_bool	ft_isalpha(int c)
{
	if (c >= 0 && c < 256)
	{
		c |= 32;
		if (c >= 'a' && c <= 'z')
			return (TRUE);
	}
	return (FALSE);
}
