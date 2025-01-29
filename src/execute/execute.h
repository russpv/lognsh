#ifndef EXECUTE_H
# define EXECUTE_H

# include "../builtins/builtins.h"
# include "../parse/parse.h"

typedef struct s_node t_ast_node;

// Fn pointer type for built-in commands
typedef int	(*t_builtin_fn)(char **args);

// map cmd to built-in fn
t_builtin_fn	get_builtin(char *command);
// exec a cmd
void			execute_command(t_ast_node *node);





#endif
