#include "execute_int.h"

//Forks (Move to execute)
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
		sig_reset_handlers();
		res = match_heredoc(m, l);
		if (0 != res)
			exit_clean(&m->list, res, __FUNCTION__, NULL);
		write_heredoc(fildes[1], l);
		exit_clean(&m->list, res, __FUNCTION__, NULL);
	}
	else
	{
		close(fildes[1]);
		waitchild(&status, 1);
		if (ERR_CHILD_SIGINT == status)
		{
			write(STDOUT_FILENO, HDOC_PROMPT"^C\n", ft_strlen(HDOC_PROMPT) + 3);
			g_last_signal = -1;
		}
		read_heredoc(fildes[0], l);
		close (fildes[0]);
		sig_set_handlers();
	}
	return (0);
}
