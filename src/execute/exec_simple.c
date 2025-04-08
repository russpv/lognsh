#include "execute_int.h"

#define EMSG_NULLARGV "ERR null argv parameters\n"
#define EMSG_NULLPATH "ERR null fullpath parameters\n"
#define EMSG_NULLTCMD "ERR null t_cmd parameters\n"

/* This execve's a simple cmd and
 * manages the various I/O redirections
 * And cleans up state (useless to child)
 */
static int	_do_child_ops(t_state *s)
{
	const t_cmd			*c = (const t_cmd *)get_cmd(s);
	t_cmd_fns *const	f = get_cmd_fns(s);
	const char			*fpath = (const char *)(f)->c_get_fullpath((t_cmd *)c);
	const char			**argv = (const char **)(f)->c_get_argv((t_cmd *)c);
	const char			**envp = (const char **)get_envp(s);

	dprint("Child: envp=%p", (void *)envp);
	if (!argv)
		return (err(EMSG_NULLARGV), -1);
	if (!fpath)
		return (err(EMSG_NULLPATH), -1);
	if (!c)
		return (err(EMSG_NULLTCMD), -1);
	if (0 != \
		(get_parse_fns(s))->p_do_redirections((f)->c_get_node((t_cmd *)c)))
		return (ERR_REDIR);
	dprint("%s: Child exec'g %s\n", _MOD_, fpath);
	if (NULL == fpath)
		return (-1);
	else if (-1 == execve(fpath, (char **)argv, (char **)envp))
		return (perror(EMSG_EXECVE), ERR_EXECVE);
	destroy_state(&s);
	return (0);
}

/* Forks, resets signal handlers, execve's, sets exit status.
 * Returns early in case SIGINT received.
 * If anything errors in the child, exits with 127.
 */
int	exec_fork_execve(t_state *s)
{
	pid_t	p;
	int		exit_code;

	if (SIGINT == g_last_signal)
		return (SIGINT_BEFORE_FORK);
	sig_ignore();
	p = fork();
	if (0 == p)
	{
		sig_reset_handlers();
		exit_code = _do_child_ops(s);
		if (0 != exit_code)
			exit_clean(&get_mem(s)->list, exit_code, NULL, NULL);
	}
	else if (p < 0)
		return (perror(EMSG_FORK), ERR_FORK);
	waitchildpid(&exit_code, p);
	if (SIGINT == exit_code)
		write(STDOUT_FILENO, "\n", 1);
	exit_code = handle_exit(s, exit_code);
	s_free_cmd(s);
	sig_set_handlers(INT_DFL);
	return (exit_code);
}
