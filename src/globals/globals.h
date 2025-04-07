#ifndef GLOBALS_H
# define GLOBALS_H

# include <signal.h>

# define SHELL_NAME "minish"
# define DFL_PROMPT "GREENminish> RESET"
# define HDOC_PROMPT "> "
# define INPUT_BUF_SZ 1024 // Used in cd, test mode argv
# define MAX_INPUT_SZ 4096
# define MAX_OUTPUT_SZ 4096 // pipes
# define MAX_NAME_LEN 50 // export names, token raws, error msgs, fns,
# define MAX_INT_BUFLEN 101 // token raws, (make larger)
# define IFS " \t\n"

extern volatile sig_atomic_t	g_last_signal;

#endif
