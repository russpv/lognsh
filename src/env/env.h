#ifndef ENV_H
# define ENV_H

#include "../data_structures/hashtable.h"


typedef struct  s_env   t_env;

// for environment management
t_env   *copy_envp(char **envp);
// gets the PATH environment var as an array for cmd_run.c
char    **s_getenv(void);
// gets value of an environment variable from linked list
char    *s_getenv_value(const char *key, t_env *env);
int     set_env_var(t_env **env, const char *key, const char *value);
int	unset_env_var(t_env **env, const char *key);
void    print_env(t_env *env);
void    free_env_list(t_env *env);

char	**lst_to_array(t_env *env_list);
// implement PATH lookup, caching
// cwd, pwd
/*
char *get_env_var(const char *key);

// add key value pair to ENV
int	set_env_var(char *key, char *value);


char	*search_path(const char *command);
char	**expand_glob(const char *pattern);

// TODO implement hash table index
// build_ht();
*/

#endif
