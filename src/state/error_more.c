#include "error.h"

// Don't like the unix error msg
void	print_redirect_warning(char *topath)
{
	write(STDERR_FILENO, SHELL_NAME, ft_strlen(SHELL_NAME));
	write(STDERR_FILENO, ": ", 2);
	write(STDERR_FILENO, "warning: An error occurred while redirecting file '",
		ft_strlen("warning: An error occurred while redirecting file '"));
	write(STDERR_FILENO, topath, ft_strlen(topath));
	write(STDERR_FILENO, "' \n", 3);
}

/* Prints "too many arguments"*/
void	print_too_many_args(const char *caller)
{
	write(STDERR_FILENO, SHELL_NAME ": ", ft_strlen(SHELL_NAME) + 2);
	if (caller)
	{
		write(STDERR_FILENO, caller, ft_strlen(caller));
		write(STDERR_FILENO, ": ", ft_strlen(": "));
	}
	write(STDERR_FILENO, "too many arguments\n",
		ft_strlen("too many arguments\n"));
}

void	print_invalid_name(const char *caller, const char *name)
{
	write(STDERR_FILENO, caller, ft_strnlen(caller, MAX_ENVVAR_LEN));
	write(STDERR_FILENO, ": ", 2);
	write(STDERR_FILENO, name, ft_strnlen(name, MAX_ENVVAR_LEN));
	write(STDERR_FILENO, ": invalid parameter name\n", \
		ft_strlen(": invalid parameter name\n"));
}

bool	is_error(int v)
{
	if (v <= ERR_STAT && v >= ERR_GENERAL)
		return (true);
	else if (v == ERR_CHILD_SIGINT)
		return (true);
	else if (v == ERR_CMD_NOT_EXEC)
		return (true);
	else if (v == ERR_CMD_NOT_FOUND)
		return (true);
	else if (v == ERR_EXIT_RANGE)
		return (true);
	return (false);
}
