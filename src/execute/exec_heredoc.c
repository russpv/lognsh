#include "execute_int.h"

int	exec_heredoc(t_state *s, t_lex *l)
{
	int		fildes[2];
	pid_t	pid;
	int		res;
	int		status;
	int		exit_code;

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
		res = (get_lex_fns(s))->lex_match_heredoc(get_mem(s), l);
		if (SIGINT != g_last_signal && ERR_RL_ABORTED == res)
		{
			print_hdoc_error(ft_itoa_mem(&get_mem(s)->list, get_mem(s)->f, 1
					+ (get_lex_fns(s))->lex_get_lines(l)),
				(get_lex_fns(s))->lex_get_eof(l));
			close(STDIN_FILENO);
			exit_code = ERR_RL_ABORTED;
		}
		else if (SIGINT == g_last_signal && ERR_RL_ABORTED == res)
			exit_code = SIGINT;
		if (ERR_RL_ABORTED == res)
		{
			close(STDOUT_FILENO);
			close(STDERR_FILENO);
			close(fildes[1]);
			exit_clean(&get_mem(s)->list, exit_code, __FUNCTION__, NULL);
		}
		write_heredoc(fildes[1], l);
		close(fildes[1]);
		exit_clean(&get_mem(s)->list, res, __FUNCTION__, NULL);
	}
	else
	{
		close(fildes[1]);
		waitchilds(&status, 1);
		if (SIGINT == status)
			write(STDOUT_FILENO, HDOC_PROMPT "^C\n", ft_strlen(HDOC_PROMPT)
				+ 3);
		read_heredoc(fildes[0], l);
		close(fildes[0]);
		sig_set_handlers(INT_DFL);
		if (ERR_CMD_GOTSIG == status)
			return (128 + g_last_signal);
	}
	return (0);
}
