
#include "signal_int.h"

void	sig_set_handlers(int option)
{
	struct sigaction	sa_int;
	struct sigaction	sa_quit;

	if (0 == isatty(STDIN_FILENO))
		return ;
	dprint(_MOD_ ": %s\n", __FUNCTION__);
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

	dprint(_MOD_ ": sig_reset_handlers\n");
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
