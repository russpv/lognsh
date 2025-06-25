#include "../include/libft.h"

/* ISASCII
** Returns non-zero if 0-0177 inclusive 
*/
t_bool	ft_isascii(int c)
{
	if (c >= 0 && c <= 127)
		return (TRUE);
	else
		return (FALSE);
}
