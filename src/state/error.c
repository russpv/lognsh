#include "error.h"
#include <unistd.h>
#include <signal.h>

/* To avoid circular includes: */
extern void set_error(t_state *s, int e);
extern int ft_strlen(const char *);

// Function to return NULL and set the error code in the state
void	*null_and_stat(t_state *s, int error_code)
{
	if (s)
	{
		set_error(s, error_code);
	}
	return (NULL);
}

const char *get_error_message(int error_code)
{
    (void)error_code;
    return (NULL);
 //TODO
}

/* Prints ": ambiguous redirect\n"*/
void print_ambiguous_redirect(const char *orig_fn)
{
    write(STDERR_FILENO, SHELL_NAME, ft_strlen(SHELL_NAME));
    write(STDERR_FILENO, ": ", 2);
    write(STDERR_FILENO, orig_fn, ft_strlen(orig_fn));
    write(STDERR_FILENO, ": ambiguous redirect\n", 21);
}

/* Prints "cmd: command not found\n"*/
void print_command_not_found(const char *cmd)
{
    write(STDERR_FILENO, SHELL_NAME, ft_strlen(SHELL_NAME));
    write(STDERR_FILENO, ": ", 2);
    if (cmd)
        write(STDERR_FILENO, cmd, ft_strlen(cmd));
    else
        write(STDERR_FILENO, "(null)", sizeof("(null)"));
    write(STDERR_FILENO, ": command not found\n", 20);
}

/* Prints ": Permission denied\n"*/
void print_permission_denied(const char *path)
{
    write(STDERR_FILENO, SHELL_NAME, ft_strlen(SHELL_NAME));
    write(STDERR_FILENO, ": ", 2);
    write(STDERR_FILENO, path, ft_strlen(path));
    write(STDERR_FILENO, ": Permission denied\n", 20);
}

/* Prints ": Buffer overflow\n"*/
void print_lex_buffer_overflow(void)
{
    write(STDERR_FILENO, SHELL_NAME, ft_strlen(SHELL_NAME));
    write(STDERR_FILENO, ": Buffer overflow\n", 18);
}
/* Prints "too many arguments"*/
void    print_too_many_args(const char *caller)
{
    write(STDERR_FILENO, SHELL_NAME, ft_strlen(SHELL_NAME));
    if (caller)
    {
        write(STDERR_FILENO, caller, ft_strlen(caller));
        write(STDERR_FILENO, ": ", 2);
    }
    write(STDERR_FILENO, "too many arguments\n", sizeof("too many arguments\n"));
}
