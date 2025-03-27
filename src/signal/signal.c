/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signal.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rpeavey <rpeavey@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/03 17:11:09 by dayeo             #+#    #+#             */
/*   Updated: 2025/03/27 18:49:46 by rpeavey          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "signal_int.h"

# define RETAIN_HIST 0
void	clear_current_line(void)
{
	rl_on_new_line();
#ifndef MACOS
	rl_replace_line("", RETAIN_HIST); // Replace the current line with an empty string
#endif
	rl_redisplay();
}

// SIGINT (Ctrl-C) handler
void	sigint_handler(int signo)
{
	if (signo == SIGINT)
	{
		g_last_signal = SIGINT;
		debug_print(_MOD_ ": I got this SIGINT.");
		write(STDOUT_FILENO, "\n", 1);
		clear_current_line();
	}
}

void	sigint_killchild(int signo)
{
	if (signo == SIGINT)
	{
		g_last_signal = SIGINT;
	}
}

// Unblocks readline() 
void	sigint_killrl(int signo)
{
	if (signo == SIGINT)
	{
		g_last_signal = SIGINT;
		close(STDIN_FILENO);
	}
}

// SIGQUIT (Ctrl-\) handler
void	sigquit_handler(int signo)
{
	if (signo == SIGQUIT)
	{
		if (isatty(STDIN_FILENO)) // ignore Ctrl-\ at prompt
			return ;
		g_last_signal = SIGQUIT; // store signal in g_signal
	}
}

void	sig_set_handlers(int option)
{
	struct sigaction	sa_int;
	struct sigaction	sa_quit;

	if (0 == isatty(STDIN_FILENO))
		return ;
	debug_print(_MOD_ ": %s\n", __FUNCTION__);
	if (INT_KCHILD == option)
		sa_int.sa_handler = sigint_killchild;
	else if (INT_DFL == option)
		sa_int.sa_handler = sigint_handler;
	else
		sa_int.sa_handler = sigint_killrl;
	sigemptyset(&sa_int.sa_mask);
	sa_int.sa_flags = SA_SIGINFO | SA_RESTART;
	if (0 != sigaction(SIGINT, &sa_int, NULL))
		perror(EMSG_SIGACTION);
	sa_quit.sa_handler = SIG_IGN;
	sigemptyset(&sa_quit.sa_mask);
	sa_quit.sa_flags = 0;
	if (0 != sigaction(SIGQUIT, &sa_quit, NULL))
		perror(EMSG_SIGACTION);
}

// Restore default action for SIGINT (terminate)
// Flush any signal masks to prevent blocking
void	sig_reset_handlers(void)
{
	struct sigaction	sa_int;
	struct sigaction	sa_quit;

	debug_print(_MOD_ ": sig_reset_handlers\n");
	sa_int.sa_handler = SIG_DFL;
	sigemptyset(&sa_int.sa_mask);
	sa_int.sa_flags = 0;
	if (0 != sigaction(SIGINT, &sa_int, NULL))
		perror(EMSG_SIGACTION);
	sa_quit.sa_handler = SIG_DFL;
	sigemptyset(&sa_quit.sa_mask);
	sa_quit.sa_flags = 0;
	if (0 != sigaction(SIGQUIT, &sa_quit, NULL))
		perror(EMSG_SIGACTION);
}

// Blocks ONLY SIGINT.
void	sig_ignore(void)
{
	struct sigaction	sa_int;

	sa_int.sa_handler = SIG_IGN;
	sa_int.sa_flags = 0;
	sigemptyset(&sa_int.sa_mask);
	if (0 != sigaction(SIGINT, &sa_int, NULL))
		perror(EMSG_SIGACTION);
}
