#include "execute_int.h"

/* Forks child and runs executor for
 * higher-level AST nodes.
 */
int	exec_fork_redirect_wait(t_state *s, t_ast_node *node, t_exec *e, t_cmd_fns *cf)
{
	pid_t	pid;
	int		status;
	int		exit_status;

	exit_status = 0;
	if (SIGINT == g_last_signal)
		return (SIGINT_BEFORE_FORK);
	pid = fork();
	if (pid < 0)
	{
		err(EMSG_FORK);
		return (ERR_FORK);
	}
	else if (0 == pid)
	{
		sig_reset_handlers();
		cf->save_redirs((t_cmd *)e->proc_cmd);
		if (0 != (get_parse_fns(s))->p_do_redirections((t_ast_node *)e->proc))
		{
			cf->restore_redirs((t_cmd *)e->proc_cmd);
			exit_clean(&get_mem(s)->list, EX_EREDIR, __FUNCTION__, NULL);
		}
		exit_status = e->executor(s, node);
		exit_clean(&get_mem(s)->list, exit_status, __FUNCTION__, NULL);
	}
	waitchilds(&status, 1);
	set_exit_status(s, status);
	sig_set_handlers(INT_DFL);
	return (exec_get_exit_status(status));
}
