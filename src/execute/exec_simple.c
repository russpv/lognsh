#include "execute_int.h"

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
		return (err("_do_child_ops: ERR null argv parameters\n"), -1);
	if (!fullpath)
		return (err("_do_child_ops: ERR null fullpath parameters\n"), -1);
	if (!c)
		return (err("_do_child_ops: ERR null t_cmd parameters\n"), -1);
	if (-1 == p_do_redirections(c_get_node((t_cmd *)c)))
		return (-1);
	debug_print("Exec: Child exec'g %s\n", fullpath);
	if (NULL == fullpath)
		return (-1);
	else if (-1 == execve(fullpath, (char **)argv, (char **)envp))
		err("exec_simple ERR execve()\n");
	destroy_state(s);
	return (0);
}

/* Handles redirects and calls built-in */
int	exec_bi_call(t_state *s, t_builtin_fn bi)
{
	const t_cmd	*c = (const t_cmd *)get_cmd(s);
	const char	**argv = (const char **)c_get_argv((t_cmd *)c);
	const int	argc = (const int)c_get_argc((t_cmd *)c);

	debug_print("Exec: exec_bi_call...\n");
	if (!argv || !bi || !c)
		return (err("exec_bi_run: ERR null command parameters\n"), -1);
	debug_print("Exec: exec_bi_call saving redir fns...\n");
	save_redirs((t_cmd *)c);
	debug_print("Exec: exec_bi_call doing redirs ...\n");
	if (-1 == p_do_redirections(c_get_node((t_cmd *)c)))
		return (-1);
	debug_print("Exec: Shell exec'g builtin\n");
	if (-1 == bi(s, (char **)argv, argc))
		debug_print("Exec: ERR bi()\n");
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
			write(STDERR_FILENO, "ERR child ops\n", sizeof("ERR child ops\n"));
			destroy_state(s);
			exit(127);
		}
	}
	else if (p < 0)
		err("ERR exec_fork_execve");
	waitchild(&exit_code, 1);
	set_exit_status(s, exit_code);
	s_free_cmd(s);
	return (exit_code);
}
