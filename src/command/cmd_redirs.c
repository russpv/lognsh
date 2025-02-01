#include "command_int.h"

int	save_redirs(t_cmd *c)
{
	if (!c)
		return (err("save_redirs: ERR null command parameters\n"), -1);
	c->saved_stderr = dup(STDERR_FILENO);
	c->saved_stdin = dup(STDIN_FILENO);
	c->saved_stdout = dup(STDOUT_FILENO);
	return (0);
}

int	restore_redirs(t_cmd *c)
{
	if (!c)
		return (err("restore_redirs: ERR null command parameters\n"), -1);
	dup2(c->saved_stderr, STDERR_FILENO);
	close(c->saved_stderr);
    c->saved_stderr = -1;
	dup2(c->saved_stdin, STDIN_FILENO);
	close(c->saved_stdin);
    c->saved_stdin = -1;
	dup2(c->saved_stdout, STDOUT_FILENO);
	close(c->saved_stdout);
    c->saved_stdout = -1;
	return (0);
}
