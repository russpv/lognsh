#ifndef BI_H
# define BI_H

# include "../state/state.h"
# include <stdio.h>
# include <unistd.h>

/* These are indexed in the lexer */
# define BI_ECHO "echo"
# define BI_CD "cd"
# define BI_PWD "pwd"
# define BI_EXPORT "export"
# define BI_UNSET "unset"
# define BI_ENV "env"
# define BI_EXIT "exit"
# define BI_EXEC "exec"
# define BI_SET "set"
# define BI_COUNT 8

/* Forwards */
typedef struct s_global_state	t_state;
typedef int						(*t_builtin_fn)(t_state *s, char **args,
							int argc);

t_builtin_fn					get_builtin(char *command);

#endif
