#include "command_int.h"

#define EMSG_RUNC_STRJ "Path strjoin err\n"
#define EMSG_RUNC_EXEC "run_cmd ERR execve()\n"
#define LOGMSG_RUNC_GOT "Cmd: Found command! at %s\n"
#define LOGMSG_RUNC_SIGINT "Cmd: Received SIGINT before fork\n"

static int	_locate_cmd(t_state *s, t_ast_node *a, t_cmd *c)
{
	int	exit_status;

	exit_status = find_and_validate_cmd(s, p_get_cmd(a), &c->fullpath);
	if (0 != exit_status)
		return (s_free_cmd(s), exit_status);
	if (c->fullpath)
		lgprint(LOGMSG_RUNC_GOT, c->fullpath);
	return (0);
}

static int	_execute(t_state *s, t_cmd *c)
{
	int	exit_status;

	if (CTXT_PIPELINE == st_int_peek(get_cmd(s)->st)
		|| CTXT_PROC == st_int_peek(get_cmd(s)->st))
	{
		if (-1 == execve(c->fullpath, c->argv, get_envp(s)))
			perror(EMSG_EXECVE);
	}
	else
	{
		exit_status = exec_fork_execve(s);
		if (SIGINT_BEFORE_FORK == exit_status)
			lgprint(LOGMSG_RUNC_SIGINT);
		return (exit_status);
	}
	return (0);
}

/* Forks depending on execution context
 * In the context of a proc or pipeline, forking would already
 * be done and be in a child process.
 * A null cmd name is valid, nothing runs.
 * Execute module handles redirects and forking in exec_fork_execve()
 */
int	run_cmd(t_state *s, t_ast_node *a)
{
	t_cmd	*c;
	int		exit_status;

	if (p_get_type(a) != AST_NODE_CMD)
		return (EINVAL);
	if (NULL == p_get_cmd(a))
		return (EXIT_NULLCMD);
	c = get_cmd(s);
	exit_status = _locate_cmd(s, a, c);
	assert(c_get_fullpath(c) != NULL);
	if (0 != exit_status)
		return (s_free_cmd(s), exit_status);
	dprint(_MOD_ ": %s: exec'g\n", __FUNCTION__);
	exit_status = _execute(s, c);
	return (s_free_cmd(s), exit_status);
}
