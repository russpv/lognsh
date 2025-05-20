#include "execute_int.h"

void	destroy_exec(t_mem_mgr *m, t_exec *e)
{
	if (e && m)
		m->dealloc(&m->list, e);
}

// Allocates and inits a t_exec
t_exec	*exec_init(t_mem_mgr *m, int cmdc, t_cmd *proc_cmd, t_ast_node *proc)
{
	t_exec	*e;

	e = m->f(&m->list, sizeof(struct s_exec) + sizeof(pid_t) * (cmdc + 1)
			+ sizeof(int) * (cmdc + 1));
	if (!e)
		exit_clean(&m->list, ENOMEM, __FUNCTION__, EMSG_MALLOC);
	e->cmdc = cmdc;
	e->proc = NULL;
	e->proc_cmd = NULL;
	if (proc)
		e->proc = proc;
	if (proc_cmd)
		e->proc_cmd = proc_cmd;
	e->executor = NULL;
	e->pids = (pid_t *)(e + 1);
	e->exit_codes = (int *)(e->pids + cmdc + 1);
	ft_memset(e->pids, 0, sizeof(pid_t) * (cmdc + 1));
	ft_memset(e->exit_codes, 0, sizeof(int) * (cmdc + 1));
	e->pids[cmdc] = -1;
	e->exit_codes[cmdc] = -1;
	return (e);
}

void	exec_set_executor(t_exec *e, t_execute_fn x)
{
	if (!e || !x)
		return ;
	e->executor = x;
}

/* Interpret waitpid() exit status (signals ignored here) */
int	exec_get_exit_status(int status)
{
	int	exit_status;
	int	signal_number;

	exit_status = 0;
	signal_number = 0;
	if (WIFEXITED(status))
	{
		exit_status = WEXITSTATUS(status);
		return (exit_status);
	}
	else if (WIFSIGNALED(status))
	{
		signal_number = WTERMSIG(status);
		return (signal_number);
	}
	return (-1);
}

int	handle_exit(t_state *s, int code)
{
	cprintf(MAGENTA, "got code:%d\n", code);
	if (EX_HAPPY == code)
		return (EX_HAPPY);
	else if (ERR_REDIR == code)
		return (set_exit_status(s, EX_EREDIR), EX_ERNDM);
	else
		return (set_exit_status(s, code), code);
}
