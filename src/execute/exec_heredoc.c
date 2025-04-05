/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_heredoc.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dayeo <dayeo@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/05 11:48:28 by dayeo             #+#    #+#             */
/*   Updated: 2025/04/05 21:02:20 by dayeo            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "execute_int.h"

static int	_heredoc_child(t_mem_mgr *m, t_lex *l, int fildes[2])
{
	int	res;
	int	exit_code;

	close(fildes[0]);
	sig_set_handlers(INT_KRL);
	res = match_heredoc(m, l);
	if (SIGINT != g_last_signal && ERR_RL_ABORTED == res)
	{
		print_hdoc_error(ft_itoa_mem(&m->list, m->f, 1 + lex_get_lines(l)), \
			lex_get_eof(l));
		exit_code = ERR_RL_ABORTED;
	}
	else if (SIGINT == g_last_signal && ERR_RL_ABORTED == res)
		exit_code = SIGINT;
	if (ERR_RL_ABORTED == res)
	{
		close(STDOUT_FILENO);
		close(STDERR_FILENO);
		close(fildes[1]);
		exit_clean(&m->list, exit_code, __FUNCTION__, NULL);
	}
	write_heredoc(fildes[1], l);
	exit_clean(&m->list, res, __FUNCTION__, NULL);
	return (0);
}

int	exec_heredoc(t_mem_mgr *m, t_lex *l)
{
	int		fildes[2];
	int		status;
	pid_t	pid;

	if (-1 == pipe(fildes))
		return (perror("pipe"), ERR_PIPE);
	sig_ignore();
	pid = fork();
	if (pid < 0)
		return (perror("fork"), ERR_FORK);
	else if (0 == pid)
		_heredoc_child(m, l, fildes);
	else
	{
		close(fildes[1]);
		waitchilds(&status, 1);
		if (SIGINT == status)
			write(STDOUT_FILENO, HDOC_PROMPT"^C\n", ft_strlen(HDOC_PROMPT) + 3);
		read_heredoc(fildes[0], l);
		close(fildes[0]);
		sig_set_handlers(INT_DFL);
		if (ERR_CMD_GOTSIG == status)
			return (128 + g_last_signal);
	}
	return (0);
}

/*int	exec_heredoc(t_mem_mgr *m, t_lex *l)
{
	int		fildes[2];
	int		res;
	int		status;
	int		exit_code;
	pid_t	pid;

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
		res = match_heredoc(m, l);
		if (SIGINT != g_last_signal && ERR_RL_ABORTED == res)
		{
			print_hdoc_error(ft_itoa_mem(&m->list, m->f, 1 + lex_get_lines(l)), \
				lex_get_eof(l));
			exit_code = ERR_RL_ABORTED;
		}
		else if (SIGINT == g_last_signal && ERR_RL_ABORTED == res)
			exit_code = SIGINT;
		if (ERR_RL_ABORTED == res)
		{
			close(STDOUT_FILENO);
			close(STDERR_FILENO);
			close(fildes[1]);
			exit_clean(&m->list, exit_code, __FUNCTION__, NULL);
		}
		write_heredoc(fildes[1], l);
		exit_clean(&m->list, res, __FUNCTION__, NULL);
	}
	else
	{
		close(fildes[1]);
		waitchilds(&status, 1);
		if (SIGINT == status)
			write(STDOUT_FILENO, HDOC_PROMPT"^C\n", ft_strlen(HDOC_PROMPT) + 3);
		read_heredoc(fildes[0], l);
		close (fildes[0]);
		sig_set_handlers(INT_DFL);
		if (ERR_CMD_GOTSIG == status)
			return (128 + g_last_signal);
	}
	return (0);
}*/
