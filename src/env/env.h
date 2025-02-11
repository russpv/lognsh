#ifndef ENV_H
# define ENV_H

# include "../include/libft.h"
# include "../data_structures/hashtable.h"


# define	SHELL_PATH "PATH"

struct s_env;
typedef struct s_env	t_env;

// for environment management
t_env					*copy_envp(char **envp);

// gets the PATH environment var as an array for cmd_run.c
char					**env_getenv(void);

// gets value of an environment variable from linked list
char					*env_getenv_value(const char *key, t_env *env);
int						env_set_var(t_env **env, const char *key,
							const char *value);
int						env_unset_var(t_env **env, const char *key);
void					env_print(t_env *env);
void					env_free(t_env *env);

char					**lst_to_array(t_env *env_list);

#endif
