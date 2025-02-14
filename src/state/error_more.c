#include "error.h"

void	print_redirect_error(char *topath)
{
	write(STDERR_FILENO, SHELL_NAME, ft_strlen(SHELL_NAME));
	write(STDERR_FILENO, ": ", 2);
	write(STDERR_FILENO, "warning: An error occurred while redirecting file '", \
		sizeof("warning: An error occurred while redirecting file '"));
	write(STDERR_FILENO, topath, ft_strlen(topath));
	write(STDERR_FILENO, "' \n", 3);
}
