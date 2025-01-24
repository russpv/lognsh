#ifndef STACK_H
# define STACK_H

# include "../../debug.h"
# include "../../include/libft.h"
# include <stdlib.h>

struct s_stack;
typedef struct s_stack	t_stack;

t_stack					*st_create(void);
void					st_destroy(t_stack *s);
int						st_push(t_stack *s, int thing);
int						st_pop(t_stack *s);
int						st_peek(t_stack *p);

#endif