
#ifndef GLOBALS_H
# define GLOBALS_H

# include <signal.h>

# define SHELL_NAME "minish"
# define DFL_PROMPT "\001\033[32m\002minish> \001\033[0m\002"
# define HDOC_PROMPT "> "

// Used in cd, test mode argv,
// upper bound to capture input for testing
# define INPUT_BUF_SZ 2048

# define MAX_INPUT_SZ 4096
# define MAX_OUTPUT_SZ 4096  // pipes
# define MAX_NAME_LEN 50     // export names, token raws, error msgs, fns,
# define MAX_VAL_LEN LEX_BUFSZ - MAX_NAME_LEN //export values
# define MAX_INT_BUFLEN 1024 // token raw len
# define IFS " \t\n"

/* Lexer */
# define LEX_MAX_TOKC 1000
# define LEX_BUFSZ 1024
# define MAX_HDOCSZ LEX_BUFSZ

extern volatile sig_atomic_t	g_last_signal;

#endif
