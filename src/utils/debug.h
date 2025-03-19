#ifndef DEBUG_H
# define DEBUG_H

# include "../../include/ansi_colors.h"

# include <assert.h>
# include <stdarg.h>
# include <stdio.h>
# include <unistd.h>

# ifdef DEBUGMODE
#  define DEBUG 1
# else
#  define DEBUG 0
# endif

typedef struct s_list	t_list;

void	debug_print(const char *format, ...);
void	log_print(const char *s, ...);
void	err(const char *s);
void	colored_printf(const char *color, const char *format, ...);
void	debug_detect_cycle(t_list *head);

void	print_array(char *arr[]);
#endif
