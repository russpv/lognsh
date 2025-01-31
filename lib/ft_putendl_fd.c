#include "../include/libft.h"


/* This write string to file descriptor with a newline */
void	ft_putendl_fd(char const *s, int fd)
{
	ft_putstr_fd(s, fd);
	ft_putchar_fd('\n', fd);
}
