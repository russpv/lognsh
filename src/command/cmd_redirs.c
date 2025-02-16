#include "command_int.h"

#define ERRMSG_REDIRSV_NULLARG "save_redirs: ERR null command parameters\n"
#define ERRMSG_REDIRRS_NULLARG "restore_redirs: ERR null command parameters\n"
#define DBGMSG_REDIRRS_ANNOUNCE "save_redirs: saving redir fns...\n"



int	save_redirs(t_cmd *c)
{
	if (!c)
		return (err(ERRMSG_REDIRSV_NULLARG), ERR_ARGS);
	if (c->redc > 0)
	{
		log_print(DBGMSG_REDIRRS_ANNOUNCE);
		c->saved_stderr = dup(STDERR_FILENO);
		if (-1 == c->saved_stderr)
			return (perror(ERRMSG_DUP), ERR_DUP);
		c->saved_stdin = dup(STDIN_FILENO);
		if (-1 == c->saved_stdin)
			return (perror(ERRMSG_DUP), ERR_DUP);
		c->saved_stdout = dup(STDOUT_FILENO);
		if (-1 == c->saved_stdout)
			return (perror(ERRMSG_DUP), ERR_DUP);
	}
	return (0);
}

int	restore_redirs(t_cmd *c)
{
	if (!c)
		return (err(ERRMSG_REDIRRS_NULLARG), ERR_ARGS);
	if (c->redc > 0)
	{
		if (!c->saved_stdin || !c->saved_stderr || !c->saved_stdout)
			return (err(ERRMSG_REDIRRS_NULLARG), ERR_ARGS);
		if (-1 == dup2(c->saved_stderr, STDERR_FILENO))
			perror(ERRMSG_DUP2);
		if (0 != close(c->saved_stderr))
			perror(ERRMSG_CLOSE);
		c->saved_stderr = -1;
		if (-1 == dup2(c->saved_stdin, STDIN_FILENO))
			perror(ERRMSG_DUP2);
		if (0 != close(c->saved_stdin))
			perror(ERRMSG_CLOSE);
		c->saved_stdin = -1;
		if (-1 == dup2(c->saved_stdout, STDOUT_FILENO))
			perror(ERRMSG_DUP2);
		if (0 != close(c->saved_stdout))
			perror(ERRMSG_CLOSE);
		c->saved_stdout = -1;
	}
	return (0);
}
