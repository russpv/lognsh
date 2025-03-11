/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_simple.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dayeo <marvin@42.fr>                       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/11 20:28:07 by dayeo             #+#    #+#             */
/*   Updated: 2025/03/11 20:28:12 by dayeo            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "execute_int.h"

#define EMSG_ECMD_NULLARGV "_do_child_ops: ERR null argv parameters\n"
#define EMSG_ECMD_NULLPATH "_do_child_ops: ERR null fullpath parameters\n"
#define EMSG_ECMD_NULLTCMD "_do_child_ops: ERR null t_cmd parameters\n"
#define DBGMSG_ECMD_DOEXEC _MOD_ ": Child exec'g %s\n"
#define DMSG_EC_GOT _MOD_ ": %s: Got %s, of %p\n"
#define DBGMSG_ECMD_DOEXEC _MOD_ ": Child exec'g %s\n"
#define EMSG_ECMD_CHILD "ERR child ops\n"
#define EMSG_ECMD_ARGS "exec_bi_run: ERR null command parameters\n"
#define DBGMSG_ECMD_REDIRSV _MOD_ ": exec_bi_call saving redir fns...\n"
#define DBGMSG_ECMD_REDIRDO _MOD_ ": exec_bi_call doing redirs ...\n"
#define DBGMSG_ECMD_DOBLTIN _MOD_ ": Shell exec'g builtin\n"
#define DBGMSG_ECMD_ERRBLTIN _MOD_ ": ERR bi()\n"

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
		return (err(EMSG_ECMD_NULLARGV), -1);
	if (!fullpath)
		return (err(EMSG_ECMD_NULLPATH), -1);
	if (!c)
		return (err(EMSG_ECMD_NULLTCMD), -1);
	if (0 != p_do_redirections(c_get_node((t_cmd *)c)))
		return (ERR_REDIR);
	debug_print(DBGMSG_ECMD_DOEXEC, fullpath);
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

	debug_print(DMSG_EC_GOT, __FUNCTION__, argv[0], a);
	if (!argv || !bi || !c || !a)
		return (err(EMSG_ECMD_ARGS), ERR_ARGS);
	save_redirs((t_cmd *)c);
	if (0 != p_do_redirections((t_ast_node *)a))
		return (ERR_REDIR);
	debug_print(DBGMSG_ECMD_DOBLTIN);
	exit_code = bi(s, (char **)argv, argvc);
	if (0 != exit_code)
		debug_print(DBGMSG_ECMD_ERRBLTIN);
	restore_redirs((t_cmd *)c);
	s_free_cmd(s);
	return (0);
}

/* Forks, resets signal handlers, execve's, sets exit status.
 * Returns early in case SIGINT received.
 * If anything errors in the child, exits with 127.
 */
/* //previous exec_fork_execve
int	exec_fork_execve(t_state *s)
{
	pid_t	p;
	int		exit_code;

	if (SIGINT == g_last_signal)
		return (SIGINT_BEFORE_FORK);
	p = fork();
	if (0 == p)
	{
		reset_signal_handlers();
		if (0 != _do_child_ops(s))
		{
			err(EMSG_ECMD_CHILD);
			destroy_state(s);
			exit(ERR_CHILD_FAILED);
		}
	}
	else if (p < 0)
	{
		perror(EMSG_FORK);
		return (ERR_FORK);
	}
	waitchild(&exit_code, 1);
	set_exit_status(s, exit_code);
	s_free_cmd(s);
	return (exit_code);
}*/

static void	ignore_or_restore_sigint(struct sigaction *sa_restore, int ignore)
{
	struct sigaction	sa;

	sigemptyset(&sa.sa_mask);
	if (ignore)
	{
		sa.sa_handler = SIG_IGN;
		sa.sa_flags = 0;
		sigaction(SIGINT, &sa, sa_restore);
	}
	else
	{
		sa.sa_handler = sa_restore->sa_handler;
		sa.sa_flags = sa_restore->sa_flags;
		sigaction(SIGINT, &sa, NULL);
	}
}

static void	exec_child_process(t_state *s)
{
	reset_signal_handlers();
	if (_do_child_ops(s) != 0)
		exit(ERR_CHILD_FAILED);
}

static void	reset_prompt(void)
{
	if (g_last_signal == SIGINT)
	{
		g_last_signal = 0;
		write(STDOUT_FILENO, "\n", 1);
		rl_on_new_line();
		rl_redisplay();
	}
}

int	exec_fork_execve(t_state *s)
{
	struct sigaction	sa_restore;
	pid_t				p;
	int					exit_code;

	if (g_last_signal == SIGINT)
	{
		g_last_signal = 0;
		return (SIGINT_BEFORE_FORK);
	}
	ignore_or_restore_sigint(&sa_restore, 1);
	p = fork();
	if (p == 0)
		exec_child_process(s);
	else if (p < 0)
	{
		perror(EMSG_FORK);
		ignore_or_restore_sigint(&sa_restore, 0);
		return (ERR_FORK);
	}
	waitchild(&exit_code, 1);
	ignore_or_restore_sigint(&sa_restore, 0);
	reset_prompt();
	set_exit_status(s, exit_code);
	s_free_cmd(s);
	return (exit_code);
}
