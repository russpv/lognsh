#include "execute_int.h"

static int	_handle_rl_errs_and_write(t_state *s, t_lex *l, int *fildes)
{
	int	rl_res;

	rl_res = (get_lex_fns(s))->lex_match_heredoc(get_mem(s), l);
	if (SH_SIGINT != g_last_signal && ERR_RL_ABORTED == rl_res)
	{
		print_hdoc_error(ft_itoa_mem(&get_mem(s)->list, get_mem(s)->f, 1
				+ (get_lex_fns(s))->lex_get_lines(l)),
			(get_lex_fns(s))->lex_get_eof(l));
		close(STDIN_FILENO);
		rl_res = ERR_RL_ABORTED;
	}
	else if (SH_SIGINT == g_last_signal && ERR_RL_ABORTED == rl_res)
		rl_res = SH_SIGINT;
	if (ERR_RL_ABORTED == rl_res)
	{
		close(STDOUT_FILENO);
		close(STDERR_FILENO);
		close(fildes[1]);
		exit_clean(&get_mem(s)->list, rl_res, __FUNCTION__, NULL);
	}
	write_heredoc(fildes[1], l);
	close(fildes[1]);
	return (rl_res);
}

static int	_wait_and_read(t_lex *l, int *fildes)
{
	int	status;

	waitchilds(&status, 1);
	if (SH_SIGINT == status)
		write(STDOUT_FILENO, HDOC_PROMPT "^C\n", ft_strlen(HDOC_PROMPT) + 3);
	read_heredoc(fildes[0], l);
	close(fildes[0]);
	sig_set_handlers(INT_DFL);
	return (status);
}

int	exec_heredoc(t_state *s, t_lex *l)
{
	int		fildes[2];
	pid_t	pid;
	int		status;

	if (-1 == pipe(fildes))
		return (perror("pipe"), ERR_PIPE);
	sig_ignore();
	pid = fork();
	if (pid < 0)
		return (perror("fork"), ERR_FORK);
	else if (0 == pid)
	{
		close(fildes[0]);
		sig_set_handlers(INT_KRL);
		status = _handle_rl_errs_and_write(s, l, fildes);
		exit_clean(&get_mem(s)->list, status, __FUNCTION__, NULL);
	}
	else
	{
		close(fildes[1]);
		status = _wait_and_read(l, fildes);
		if (ERR_CMD_GOTSIG == status)
			return (128 + g_last_signal);
	}
	return (status);
}
