// Probably need error handler with value map to messages

#include "error.h"
#include <unistd.h>

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

void print_command_not_found(const char *cmd)
{
    fprintf(stderr, "%s: %s: command not found\n", SHELL_NAME, cmd);
}

void    print_permission_denied(const char *path)
{
    fprintf(stderr, "%s: %s: Permission denied", SHELL_NAME, path);
}
