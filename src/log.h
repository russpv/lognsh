#ifndef LOG_H
# define LOG_H

# include <assert.h>
# include <stdarg.h>
# include <stdio.h>
# include <unistd.h>

# ifdef LOGMODE
#  define LOGGING 1
# else
#  define LOGGING 0
# endif

typedef struct s_cmd			t_cmd;
typedef struct s_node			t_ast_node;

void	log_command_info(t_cmd *c, t_ast_node *a);

#endif