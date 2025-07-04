#include "../include/libft.h"

/* This prints integers.
** GCC has a problem with writeing more than 1 byte
*/
void	ft_putnbr(int num)
{
	if (num == (int)1 << (sizeof(int) * 8 - 1))
		write(1, &(char){num + '0'}, 1);
	if (num < 0)
	{
		write(1, "-", 1);
		num *= -1;
	}
	if (num >= 10)
	{
		ft_putnbr(num / 10);
		ft_putnbr(num % 10);
	}
	if (num < 10)
		write(1, &(char){num + '0'}, 1);
}
