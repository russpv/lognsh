#include "execute_int.h"

int	exec_heredoc(t_mem_mgr *m, t_lex *l)
{
	int fildes[2];
	pid_t pid;
	int res;
	int status;

	if (-1 == pipe(fildes))
		return (perror("pipe"), ERR_PIPE);
	sig_ignore();
	pid = fork();
	if (pid < 0)
		return (perror("Fork"), ERR_FORK);
	else if (0 == pid)
	{
		close(fildes[0]);
		sig_set_handlers(INT_KRL);
		res = match_heredoc(m, l);
		if (SIGINT == g_last_signal || res == ERR_CMD_SIGINTD)
		{
			close(STDOUT_FILENO);
			close(STDERR_FILENO);
			close(fildes[1]);
			exit_clean(&m->list, ERR_CMD_SIGINTD, __FUNCTION__, NULL);
		}
		write_heredoc(fildes[1], l);
		exit_clean(&m->list, res, __FUNCTION__, NULL);
	}
	else
	{
		close(fildes[1]);
		waitchild(&status, 1);
		if (ERR_CMD_SIGINTD == status)
			write(STDOUT_FILENO, HDOC_PROMPT"^C\n", ft_strlen(HDOC_PROMPT) + 3);
		else
			read_heredoc(fildes[0], l);
		close (fildes[0]);
		sig_set_handlers(INT_DFL);
	}
	return (0);
}
