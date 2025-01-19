#include "libft.h"

/* ISSPACE
** Returns 1 if ch is a space char
*/

/* \t == 9, \n == 10, \f == , 12, \r == 13, \t == 12, ' ' == 32, \v ==  11 */
t_bool	ft_isspace(int ch)
{
	return ((ch >= '\t' && ch <= '\r') || (ch == ' '));
}
