#ifndef DEBUG_H
# define DEBUG_H

# include "data_structures/llist.h"
# include <assert.h>
# include <stdarg.h>
# include <stdio.h>
# include <unistd.h>

# ifdef DEBUGMODE
#  define DEBUG 1
# else
#  define DEBUG 0
# endif

# define LOG 1
# define RESET "\033[0m"
# define GREEN "\033[32m"
# define BLUE "\033[34m"
# define RED "\033[1;31m"
# define YELLOW "\033[1;33m"
# define MAGENTA "\033[1;35m"
# define CYAN "\033[1;36m"

void	debug_print(const char *format, ...);
void	log_printf(const char *s, ...);
void	err(const char *s);
void	colored_printf(const char *color, const char *format, ...);

#endif
