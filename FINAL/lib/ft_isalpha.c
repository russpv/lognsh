#include "../include/libft.h"

/* ISALPHA
** Tests if char is alphabetic
** behavior is undefined if not passed as unsigned char
** but we return 0 here
*/

t_bool	ft_isalpha(int c)
{
	unsigned char	uc;

	uc = (unsigned char)c;
	if ((uc >= 'A' && uc <= 'Z') || (uc >= 'a' && uc <= 'z'))
		return (TRUE);
	return (FALSE);
}
