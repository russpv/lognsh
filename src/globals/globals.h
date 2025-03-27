#ifndef GLOBALS_H
# define GLOBALS_H

# include <signal.h>

# define SHELL_NAME "minish"
# define DFL_PROMPT GREEN SHELL_NAME "> " RESET
# define HDOC_PROMPT "> "
# define INPUT_BUF_SZ 1024
# define IFS " \t\n"

extern volatile sig_atomic_t	g_last_signal;

#endif