#include "stack.h"

#define MAX_ST_DEPTH 10

struct					s_stack
{
	int					st[MAX_ST_DEPTH];
	int					depth;
};

typedef struct s_stack	t_stack;
