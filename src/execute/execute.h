#ifndef EXECUTE_H
# define EXECUTE_H

# include "../builtins/builtins.h"
# include "../parse/parse.h"

// Fn pointer type for built-in commands
typedef void	(*t_builtin_fn)(char **args);

// map cmd to built-in fn
t_builtin_fn	get_builtin(char *command);
// exec a cmd
void			execute_command(t_ast_node *node);
// convert linked lst of args to array
char			**list_to_array(t_list *args, int argc);

#endif
