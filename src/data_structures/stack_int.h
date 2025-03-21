#include "stack.h"

#define MAX_ST_DEPTH 10
#include "../utils/debug.h"

struct						s_int_stack
{
	int						st[MAX_ST_DEPTH];
	int						depth;
};

typedef struct s_int_stack	t_int_stack;

struct						s_ptr_stack
{
	void					*st[MAX_ST_DEPTH];
	int						depth;
};

typedef struct s_ptr_stack	t_ptr_stack;
