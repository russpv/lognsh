#include "execute_int.h"

#define ERRMSG_ECMD_NULLARGV "_do_child_ops: ERR null argv parameters\n"
#define ERRMSG_ECMD_NULLPATH "_do_child_ops: ERR null fullpath parameters\n"
#define ERRMSG_ECMD_NULLTCMD "_do_child_ops: ERR null t_cmd parameters\n"
#define DBGMSG_ECMD_DOEXEC "Exec: Child exec'g %s\n"
#define DBGMSG_ECMD_ANNOUNCE "Exec: exec_bi_call...\n"
#define DBGMSG_ECMD_DOEXEC "Exec: Child exec'g %s\n"
#define ERRMSG_ECMD_CHILD "ERR child ops\n"
#define ERRMSG_ECMD_ARGS "exec_bi_run: ERR null command parameters\n"
#define DBGMSG_ECMD_REDIRSV "Exec: exec_bi_call saving redir fns...\n"
#define DBGMSG_ECMD_REDIRDO "Exec: exec_bi_call doing redirs ...\n"
#define DBGMSG_ECMD_DOBLTIN "Exec: Shell exec'g builtin\n"
#define DBGMSG_ECMD_ERRBLTIN "Exec: ERR bi()\n"

/* This execve's a simple cmd and
 * manages the various I/O redirections
 * And cleans up state (useless to child)
 */
static int	_do_child_ops(t_state *s)
{
	const t_cmd	*c = (const t_cmd *)get_cmd(s);
	const char	*fullpath = (const char *)c_get_fullpath((t_cmd *)c);
	const char	**argv = (const char **)c_get_argv((t_cmd *)c);
	const char	**envp = (const char **)get_envp(s);

	if (!argv)
		return (err(ERRMSG_ECMD_NULLARGV), -1);
	if (!fullpath)
		return (err(ERRMSG_ECMD_NULLPATH), -1);
	if (!c)
		return (err(ERRMSG_ECMD_NULLTCMD), -1);
	if (-1 == p_do_redirections(c_get_node((t_cmd *)c)))
		return (-1);
	debug_print(DBGMSG_ECMD_DOEXEC, fullpath);
	if (NULL == fullpath)
		return (-1);
	else if (-1 == execve(fullpath, (char **)argv, (char **)envp))
		err(ERRMSG_EXECVE);
	destroy_state(s);
	return (0);
}

/* Handles redirects and calls built-in */
int	exec_bi_call(t_state *s, t_builtin_fn bi)
{
	const t_cmd	*c = (const t_cmd *)get_cmd(s);
	const char	**argv = (const char **)c_get_argv((t_cmd *)c);
	const int	argc = (const int)c_get_argc((t_cmd *)c);

	debug_print(DBGMSG_ECMD_ANNOUNCE);
	if (!argv)
		fprintf(stderr, "SHIT\n");
	if (!argv || !bi || !c)
		return (err(ERRMSG_ECMD_ARGS), -1);
	debug_print(DBGMSG_ECMD_REDIRSV);
	save_redirs((t_cmd *)c);
	debug_print(DBGMSG_ECMD_REDIRDO);
	if (-1 == p_do_redirections(c_get_node((t_cmd *)c)))
		return (-1);
	debug_print(DBGMSG_ECMD_DOBLTIN);
	if (-1 == bi(s, (char **)argv, argc))
		debug_print(DBGMSG_ECMD_ERRBLTIN);
	restore_redirs((t_cmd *)c);
	s_free_cmd(s);
	return (0);
}

/* Forks, resets signal handlers, execve's, sets exit status.
 * Returns early in case SIGINT recieved.
 */
int	exec_fork_execve(t_state *s)
{
	pid_t	p;
	int		exit_code;

	if (SIGINT == g_last_signal)
		return (SIGINT_BEFORE_FORK);
	p = fork();
	if (0 == p)
	{
		reset_signal_handlers();
		if (-1 == _do_child_ops(s))
		{
			err(ERRMSG_ECMD_CHILD);
			destroy_state(s);
			exit(127);
		}
	}
	else if (p < 0)
		err(ERRMSG_FORK);
	waitchild(&exit_code, 1);
	set_exit_status(s, exit_code);
	s_free_cmd(s);
	return (exit_code);
}
