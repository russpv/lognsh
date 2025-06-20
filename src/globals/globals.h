
#ifndef GLOBALS_H
# define GLOBALS_H

# include <signal.h>

# define SHELL_NAME "lognsh"
# define DFL_PROMPT "\001\033[32m\002minish> \001\033[0m\002"
# define HDOC_PROMPT "> "

// used in cd, test mode argv, token raw str copying
# define INPUT_BUF_LIM 2048 // what we're willing to work with

# define MAX_INPUT_SZ 4096   // hard cap for raw input
# define MAX_OUTPUT_SZ 4096  // pipes
# define MAX_FILENAME_LEN 255
# define MAX_NAME_LEN 128     // err msgs, tok splits
# define MAX_ENV_NAME_LEN 256
# define MAX_ENV_VAL_LEN (LEX_BUFSZ - MAX_ENV_NAME_LEN)
# define MAX_INT_BUFLEN 1024 // hard cap for tok raw

/* Lexer */
# define LEX_MAX_TOKC 1000  // can be wordy
# define LEX_BUFSZ 1024		// conservative, I know
# define MAX_HDOCSZ LEX_BUFSZ // rn, hdocs are stuffed into lex_buf

# define IFS " \t\n"

extern volatile sig_atomic_t	g_last_signal;

#endif
