#ifndef COMMAND_INT_H
# define COMMAND_INT_H

#include "command.h"

/* Holds context for current command */
struct s_cmd {
	t_stack	*st; //parse depth 
	char *fullpath;
	char **argv;
	t_ast_node *curr_node;
	int saved_stdout;
	int	saved_stdin;
	int	saved_stderr;
};

typedef struct s_cmd	t_cmd;

char **c_argstoargv(char **args, char *cmd, int argc);


#endif
