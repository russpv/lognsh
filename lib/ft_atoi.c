#include "../include/libft.h"

/* ATOI
** Returns int value of byte string.
** Skips any isspace(3)
** No error checking.
*/
long	ft_atoi(const char *s)
{
	long	num;
	int		flag;

	flag = 1;
	num = 0;
	while (ft_isspace(*s))
		s++;
	if (*s == '-' || *s == '+')
	{
		if (*s == '-')
			flag *= -1;
		s++;
	}
	while (ft_isdigit(*s))
	{
		num *= 10;
		num += *s - '0';
		s++;
	}
	return (flag * num);
}
