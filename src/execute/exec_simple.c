#include "execute_int.h"


/* This executes a simple cmd and
 * manages the various I/O redirections
 */
static int	_do_child_ops(t_state *s)
{
	const t_cmd *c = (const t_cmd *)get_cmd(s);
	const char *fullpath = (const char *)c_getfullpath((t_cmd*)c);
	const char **argv = (const char **)c_getargv((t_cmd*)c);

	if (!argv || !fullpath || !c)
		return (err("ERR null command parameters\n"), -1);
	if (-1 == p_do_redirections(c_getnode((t_cmd*)c)))
		return (-1);
	debug_print("Child exec'g %s\n", fullpath);
	if (NULL == fullpath)
		return (-1);
	else if (-1 == execve(fullpath, (char**)argv, get_envp(s)))
		err("ERR execve()\n");
	return (0);
}

// TODO cleanup
int	fork_and_run(t_state *s)
{
	pid_t p = fork();

	if (0 == p)
	{
		if (-1 == _do_child_ops(s))
		{
			fprintf(stderr, "ERR child ops\n");
			exit(127); //cleanup
		}
	}
	else if (p < 0)
		err("ERR Fork");
	waitchild(get_status(s), 1);
	//cleanup(&st);
	return (get_exit_status(*get_status(s)));
}
