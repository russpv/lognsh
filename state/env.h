#ifndef ENV_H
# define ENV_H

// implement PATH lookup, caching
// cwd, pwd

get_env_var(char *key);

// add key value pair to ENV
set_env_var(char *key, char *value);

#endif
