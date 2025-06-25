#ifndef GLOBALS_H
# define GLOBALS_H

# include <signal.h>

# define SHELL_NAME "lognsh"
# define DFL_PROMPT "\001\033[32m\002" SHELL_NAME "> " "\001\033[0m\002"
# define HDOC_PROMPT "> "

// Used in cd, test mode argv,
// upper bound to capture input for testing
# define INPUT_BUF_LIM 2048

# define MAX_INPUT_SZ 4096
# define MAX_OUTPUT_SZ 4096  // pipes
# define MAX_NAME_LEN 50     // export names, token raws, error msgs, fns,
# define MAX_INT_BUFLEN 1024 // token raw len
# define IFS " \t\n"

typedef enum {
	DEBUG_OFF = 0,
	DEBUG_LOG = 1, // logging
	DEBUG_ONE = 2, // debug
	DEBUG_VER = 3, // verbose
	DEBUG_VVR = 4  // very verbose
} e_dbg_lvl; 

extern volatile sig_atomic_t	g_last_signal;

void set_debug_level(e_dbg_lvl lvl);
e_dbg_lvl get_debug_level(void);

#endif
