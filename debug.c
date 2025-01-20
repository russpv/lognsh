#include "debug.h"

void debug_print(const char *s, ...) 
{
	if (DEBUG)
	{
    	va_list args;
    	va_start(args, s);

    	vfprintf(stderr, s, args);

    	va_end(args);
	}
}

void log_printf(const char *s, ...)
{
	if (LOG)
	{
    	va_list args;
    	va_start(args, s);

    	vfprintf(stdout, s, args);

    	va_end(args);
	}
}