#include "signal_int.h"

void	clear_current_line(void)
{
	rl_on_new_line();
#ifndef MACOS // TODO REMOVE
	rl_replace_line("", RETAIN_HIST);
	// Replace the current line with an empty string
#endif
	rl_redisplay();
}

// SIGINT (Ctrl-C) handler
void	sigint_handler(int signo)
{
	if (signo == SIGINT)
	{
		g_last_signal = SIGINT;
		dprint(_MOD_ ": I got this SIGINT.");
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
		if (isatty(STDIN_FILENO))
			return ;
		g_last_signal = SIGQUIT;
	}
}
