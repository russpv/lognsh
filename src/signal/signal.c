/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signal.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rpeavey <rpeavey@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/10 16:34:31 by rpeavey           #+#    #+#             */
/*   Updated: 2025/04/12 14:05:28 by rpeavey          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "signal_int.h"

void	clear_current_line(void)
{
	rl_on_new_line();
	rl_replace_line("", RETAIN_HIST);
	rl_redisplay();
}

// SIGINT (Ctrl-C) handler
void	sigint_handler(int signo)
{
	if (signo == SIGINT)
	{
		g_last_signal = SH_SIGINT;
		dprint(_MOD_ ": I got this SIGINT.");
		write(STDOUT_FILENO, "\n", 1);
		clear_current_line();
	}
}

void	sigint_killchild(int signo)
{
	if (signo == SIGINT)
	{
		g_last_signal = SH_SIGINT;
	}
}

// Unblocks readline()
void	sigint_killrl(int signo)
{
	if (signo == SIGINT)
	{
		g_last_signal = SH_SIGINT;
		close(STDIN_FILENO);
	}
}

// SIGQUIT (Ctrl-\) handler
void	sigquit_handler(int signo)
{
	if (signo == SIGQUIT)
	{
		if (isatty(STDIN_FILENO))
			return ;
		g_last_signal = SH_SIGQUIT;
	}
}
