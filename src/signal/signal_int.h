#ifndef SIGNAL_INT_H
# define SIGNAL_INT_H

# include "signal.h"

# define _MOD_ "Signal"
# define RETAIN_HIST 0

typedef struct s_signal_manager
{
	struct sigaction	sa_int;
	struct sigaction	sa_quit;
}		t_signal_mgr;

/* Handles SIGINT (Ctrl-C) and SIGQUIT (Ctrl-\)(defeats)*/

void	sigint_handler(int signo);
void	sigquit_handler(int signo);
void	sigint_killchild(int signo);
void	sigint_killrl(int signo);

#endif
