#ifndef COMMAND_INT_H
# define COMMAND_INT_H

#include "command.h"

/* Holds context for current command */
struct s_cmd {
	t_stack	*st;
	char *fullpath;
	char **argv;
	t_ast_node *curr_node;
};

typedef struct s_cmd	t_cmd;

char **c_argstoargv(char **args, char *cmd, int argc);


#endif
