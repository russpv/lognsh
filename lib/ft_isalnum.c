#include "../include/libft.h"

/* ISALNUM
** Tests if char is alphanumeric 
*/

t_bool	ft_isalnum(int c)
{
	return (ft_isdigit(c) || ft_isalpha(c));
}
