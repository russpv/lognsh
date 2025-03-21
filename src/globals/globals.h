#ifndef GLOBALS_H
# define GLOBALS_H

# include <signal.h>

# define SHELL_NAME "minish"
# define PROMPT GREEN SHELL_NAME "> " RESET
# define INPUT_BUF_SZ 1024

extern volatile sig_atomic_t	g_last_signal;

#endif