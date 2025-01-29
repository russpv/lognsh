#ifndef ERROR_H
# define ERROR_H

// no includes, but redefs, due to circular dependency
typedef struct s_global_state t_state; 

# define ERR_TOKEN 0

void* null_and_stat(t_state* s, int error_code);

#endif