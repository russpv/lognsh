#include "command_int.h"

int	save_redirs(t_cmd *c)
{
	if (!c)
		return (err("ERR null command parameters\n"), -1);
	c->saved_stderr = dup(STDERR_FILENO);
    c->saved_stdin = dup(STDIN_FILENO);
    c->saved_stdout = dup(STDOUT_FILENO);
    return (0);
}

int	restore_redirs(t_cmd *c)
{
	if (!c)
		return (err("ERR null command parameters\n"), -1);
    dup2(c->saved_stderr, STDERR_FILENO);
    close(c->saved_stderr);
    dup2(c->saved_stdin, STDIN_FILENO);
    close(c->saved_stdin);
    dup2(c->saved_stdout, STDOUT_FILENO);
    close(c->saved_stdout);
    return (0);
}