/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signal.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rpeavey <rpeavey@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/03 17:11:09 by dayeo             #+#    #+#             */
/*   Updated: 2025/02/08 01:50:53 by rpeavey          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "signal_int.h"

// SIGINT (Ctrl-C) handler
void	sigint_handler(int signo)
{
	if (signo == SIGINT)
	{
        write(STDOUT_FILENO, "\n", 1);
        rl_replace_line("", 0);
        rl_on_new_line();
        rl_redisplay();
        // g_signal = SIGINT; // store signal in glob. var.
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
		//write(STDOUT_FILENO, "Quit (core dumped) (to bash purist: but not really)\n", 19); // RKP: the default this overrides dumps core, but the subject says do nothing
	}
}

void	set_signal_handlers(void)
{
	struct sigaction	sa_int;
	struct sigaction	sa_quit;

	sa_int.sa_handler = sigint_handler;
	sigemptyset(&sa_int.sa_mask);
	sa_int.sa_flags = SA_RESTART;
	sigaction(SIGINT, &sa_int, NULL);
	sa_quit.sa_handler = SIG_IGN;
	sigemptyset(&sa_quit.sa_mask);
	sa_quit.sa_flags = 0;
	sigaction(SIGQUIT, &sa_quit, NULL);
}

void	reset_signal_handlers(void)
{
	debug_print("reset_signal_handlers\n");
///	signal(SIGINT, SIG_DFL);  
	//signal(SIGQUIT, SIG_DFL); 

	    // Reset SIGINT to default action (usually terminate the process)
    struct sigaction sa_int;
    sa_int.sa_handler = SIG_DFL;        // Set the handler to the default action
    sigemptyset(&sa_int.sa_mask);       // Clear the signal mask (no signals are blocked during handler)
    sa_int.sa_flags = 0;                // No special flags
    sigaction(SIGINT, &sa_int, NULL);   // Apply the SIGINT handler

    // Reset SIGQUIT to default action (usually terminate the process and dump core)
    struct sigaction sa_quit;
    sa_quit.sa_handler = SIG_DFL;       // Set the handler to the default action
    sigemptyset(&sa_quit.sa_mask);      // Clear the signal mask
    sa_quit.sa_flags = 0;               // No special flags
    sigaction(SIGQUIT, &sa_quit, NULL); // Apply the SIGQUIT handler
}

int	handle_last_signal(void)
{
	if (SIGINT == g_last_signal)
		return (128 + SIGINT);
	return (0);
}