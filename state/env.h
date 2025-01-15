#ifndef ENV_H
# define ENV_H

// implement PATH lookup, caching
// cwd, pwd

get_env_var(const char *key);

// add key value pair to ENV
set_env_var(char *key, char *value);


char	*search_path(const char *command);
char	**expand_glob(const char *pattern);

// TODO implement hash table index
// build_ht();

#endif
