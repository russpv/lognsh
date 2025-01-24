#ifndef DEBUG_H
# define DEBUG_H

#include <stdio.h>
#include <stdarg.h>
#include <assert.h>

#include "src/data_structures/llist.h"

#ifdef DEBUGMODE
	#define DEBUG 1
#else
	#define DEBUG 0
#endif

#define LOG 1

void debug_print(const char *format, ...);
void log_printf(const char *s, ...);
void	err(const char *s);
#endif
