/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signal.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rpeavey <rpeavey@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/03 17:11:09 by dayeo             #+#    #+#             */
/*   Updated: 2025/03/19 16:53:40 by rpeavey          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "signal_int.h"

// SIGINT (Ctrl-C) handler
void	sigint_handler(int signo)
{
	if (signo == SIGINT)
	{
		g_last_signal = SIGINT;
		debug_print(_MOD_ ": I got this SIGINT.");
		write(STDOUT_FILENO, "\n", 1);
#ifndef	MACOS //TODO remove in final
		rl_replace_line("", 1);
        rl_on_new_line();
        rl_redisplay();
#endif
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

void	sig_set_handlers(void)
{
	struct sigaction	sa_int;
	struct sigaction	sa_quit;

	if (0 == isatty(STDIN_FILENO))
		return;
	debug_print(_MOD_ ": %s\n", __FUNCTION__);
	sa_int.sa_handler = sigint_handler;
	sigemptyset(&sa_int.sa_mask);
	sa_int.sa_flags = SA_RESTART;
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
    struct sigaction sa_int;
    struct sigaction sa_quit;

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
	struct sigaction sa_int;

	sa_int.sa_handler = SIG_IGN;
	sa_int.sa_flags = 0;
	sigemptyset(&sa_int.sa_mask);
	if (0 != sigaction(SIGINT, &sa_int, NULL))
		perror(EMSG_SIGACTION);
}
