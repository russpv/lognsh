#include "execute_int.h"

/* This execve's a simple cmd and
 * manages the various I/O redirections
 */
static int	_do_child_ops(t_state *s)
{
	const t_cmd	*c = (const t_cmd *)get_cmd(s);
	const char	*fullpath = (const char *)c_getfullpath((t_cmd *)c);
	const char	**argv = (const char **)c_getargv((t_cmd *)c);

	if (!argv)
		return (err("_do_child_ops: ERR null argv parameters\n"), -1);
	if (!fullpath)
		return (err("_do_child_ops: ERR null fullpath parameters\n"), -1);
	if (!c)
		return (err("_do_child_ops: ERR null t_cmd parameters\n"), -1);
	if (-1 == p_do_redirections(c_getnode((t_cmd *)c)))
		return (-1);
	debug_print("Child exec'g %s\n", fullpath);
	if (NULL == fullpath)
		return (-1);
	else if (-1 == execve(fullpath, (char **)argv, get_envp(s)))
		err("ERR execve()\n");
	return (0);
}

/* Handles redirects and calls built-in */
int	exec_bi_call(t_state *s, t_builtin_fn bi)
{
	const t_cmd	*c = (const t_cmd *)get_cmd(s);
	const char	**argv = (const char **)c_getargv((t_cmd *)c);
	const int	argc = (const int)c_get_argc((t_cmd *)c);

	if (!argv || !bi || !c)
		return (err("exec_bi_run: ERR null command parameters\n"), -1);
	save_redirs((t_cmd *)c);
	if (-1 == p_do_redirections(c_getnode((t_cmd *)c)))
		return (-1);
	debug_print("Shell exec'g builtin\n");
	if (-1 == bi(s, (char **)argv, argc))
		err("ERR bi()\n");
	restore_redirs((t_cmd *)c);
	return (0);
}

// TODO cleanup
int	exec_fork_execve(t_state *s)
{
	pid_t	p;

	p = fork();
	if (0 == p)
	{
		if (-1 == _do_child_ops(s))
		{
			fprintf(stderr, "ERR child ops\n");
			exit(127); // cleanup
		}
	}
	else if (p < 0)
		err("ERR exec_fork_execve");
	waitchild(get_status(s), 1);
	// cleanup(&st);
	return (get_exit_status(*get_status(s)));
}
