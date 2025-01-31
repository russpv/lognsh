#include "../include/libft.h"

/* ISDIGIT
** Tests if char is a numeric
** Note: isdigit() returns 2048 if numeric
** don't ask why.
*/

t_bool	ft_isdigit(int c)
{
	if (c >= '0' && c <= '9')
		return (TRUE);
	else
		return (FALSE);
}
