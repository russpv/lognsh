#include "execute_int.h"

#define EMSG_NULLARGV "%s: ERR null argv parameters\n"
#define EMSG_NULLPATH "%s: ERR null fullpath parameters\n"
#define EMSG_NULLTCMD "%s: ERR null t_cmd parameters\n"
#define EMSG_NULL "%s: ERR null parameters\n"
#define DMSG_DOEXEC _MOD_ ": %s: Child exec'g %s\n"
#define DMSG_GOT _MOD_ ": %s: Got %s, of %p\n"
#define EMSG_CHILD "ERR child ops\n"
#define DMSG_REDIRSV _MOD_ ": %s: saving redir fns...\n"
#define DMSG_REDIRDO _MOD_ ": %s: doing redirs ...\n"
#define DMSG_DOBLTIN _MOD_ ": %s: Shell exec'g builtin\n"
#define DMSG_ERRBLTIN _MOD_ ": %s: ERR bi()\n"

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
		return (err(EMSG_NULLARGV), -1);
	if (!fullpath)
		return (err(EMSG_NULLPATH), -1);
	if (!c)
		return (err(EMSG_NULLTCMD), -1);
	if (0 != p_do_redirections(c_get_node((t_cmd *)c)))
		return (ERR_REDIR);
	debug_print(DMSG_DOEXEC, fullpath);
	if (NULL == fullpath)
		return (-1);
	else if (-1 == execve(fullpath, (char **)argv, (char **)envp))
		return (perror(EMSG_EXECVE), ERR_EXECVE);
	destroy_state(s);
	return (0);
}

/* Executes redirects, if any, and calls built-in */
int	exec_bi_call(t_state *s, t_builtin_fn bi)
{
	int					exit_code;
	const t_cmd			*c = (const t_cmd *)get_cmd(s);
	const char			**argv = (const char **)c_get_argv((t_cmd *)c);
	const int			argvc = (const int)c_get_argvc((t_cmd *)c);
	const t_ast_node	*a = (const t_ast_node *)c_get_node((t_cmd *)c);

	debug_print(DMSG_GOT, __FUNCTION__, argv[0], a);
	if (!argv || !bi || !c || !a)
		return (err(EMSG_NULL), ERR_ARGS);
	save_redirs((t_cmd *)c);
	if (0 != p_do_redirections((t_ast_node *)a))
		return (ERR_REDIR);
	debug_print(DMSG_DOBLTIN, __FUNCTION__);
	exit_code = bi(s, (char **)argv, argvc);
	if (0 != exit_code)
		debug_print(DMSG_ERRBLTIN, __FUNCTION__);
	restore_redirs((t_cmd *)c);
	set_exit_status(s, exit_code); //update s->current_exit_code
	s_free_cmd(s);
	return (exit_code); // return bi exit codes
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
		if (0 != _do_child_ops(s))
			exit_clean(&get_mem(s)->list, ERR_CHILD_FAILED, __FUNCTION__, EMSG_CHILD);
	}
	else if (p < 0)
		return (perror(EMSG_FORK), ERR_FORK);
	waitchild(&exit_code, 1);
	set_exit_status(s, exit_code);
	if (SIGINT == exit_code)
		write(STDOUT_FILENO, "\n", 1);
	s_free_cmd(s);
	sig_set_handlers();
	return (exit_code);
}
