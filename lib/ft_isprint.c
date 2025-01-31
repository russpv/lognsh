#include "../include/libft.h"

/* ISPRINT
** returns non-zero if printable char 
*/

t_bool	ft_isprint(int c)
{
	if (c >= ' ' && c <= '~')
		return (TRUE);
	else
		return (FALSE);
}
