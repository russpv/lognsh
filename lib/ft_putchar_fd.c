#include "libft.h"

/* This write to the passed file descriptor instead of STDOUT */
void	ft_putchar_fd(char c, int fd)
{
	write(fd, &c, 1);
}
