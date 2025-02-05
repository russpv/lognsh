// Probably need error handler with value map to messages

#include "error.h"
#include <unistd.h>
#include <signal.h>

volatile sig_atomic_t g_signal = 0; 

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
    switch (error_code)
    {
    case ERR_MEM:
        return "Memory allocation failed";
    case ERR_SYNTAX:
        return "Syntax error";
    case ERR_FILE_NOT_FOUND:
        return "File not found";
    default:
        return "Unknown error";
    }
}

void print_ambiguous_redirect(const char *orig_fn)
{
    write(STDERR_FILENO, SHELL_NAME, ft_strlen(SHELL_NAME));
    write(STDERR_FILENO, ": ", 2);
    write(STDERR_FILENO, orig_fn, ft_strlen(orig_fn));
    write(STDERR_FILENO, ": ambiguous redirect\n", 21);
}

void print_command_not_found(const char *cmd, const char *caller)
{
    write(STDERR_FILENO, SHELL_NAME, ft_strlen(SHELL_NAME));
    write(STDERR_FILENO, ": ", 2);
    if (caller)
    {
        write(STDERR_FILENO, caller, ft_strlen(caller));
        write(STDERR_FILENO, ": ", 2);
    }
    write(STDERR_FILENO, cmd, ft_strlen(cmd));
    write(STDERR_FILENO, ": command not found\n", 20);
}

void print_permission_denied(const char *path)
{
    write(STDERR_FILENO, SHELL_NAME, ft_strlen(SHELL_NAME));
    write(STDERR_FILENO, ": ", 2);
    write(STDERR_FILENO, path, ft_strlen(path));
    write(STDERR_FILENO, ": Permission denied\n", 20);
}

void print_lex_buffer_overflow(void)
{
    write(STDERR_FILENO, SHELL_NAME, ft_strlen(SHELL_NAME));
    write(STDERR_FILENO, ": Buffer overflow\n", 18);
}


/*
void    print_ambiguous_redirect(const char *orig_fn)
{
    fprintf(stderr, "%s: %s: ambiguous redirect\n", SHELL_NAME, orig_fn);
}

void print_command_not_found(const char *cmd, const char *caller)
{
    if (caller)
        fprintf(stderr, "%s: %s: %s: not found\n", SHELL_NAME, caller, cmd);
    else
        fprintf(stderr, "%s: %s: command not found\n", SHELL_NAME, cmd);
}

void    print_permission_denied(const char *path)
{
    fprintf(stderr, "%s: %s: Permission denied\n", SHELL_NAME, path);
}

void print_lex_buffer_overflow(void)
{
    fprintf(stderr, "%s: Buffer overflow\n", SHELL_NAME);
}
*/