#ifndef ERROR_H
# define ERROR_H

#include "state.h"

#define ERR_NONE 0
#define ERR_MEMORY 1
#define ERR_SYNTAX 2
#define ERR_FILE_NOT_FOUND 3
#define ERR_TOKEN 4

extern sig_atomic_t _g_signal;


// no includes, but redefs, due to circular dependency
typedef struct s_global_state t_state; 



void* null_and_stat(t_state* s, int error_code);
const char *get_error_message(int error_code);


#endif