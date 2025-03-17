#ifndef STACK_H
# define STACK_H

# include "../../include/libft.h"
# include "i_llist.h"

# include <stdlib.h>

struct						s_int_stack;
typedef struct s_int_stack	t_int_stack;

t_int_stack					*st_int_create(t_mem_mgr *m);
void						st_int_destroy(t_mem_mgr *m, t_int_stack *s);
int							st_int_push(t_int_stack *s, int thing);
int							st_int_pop(t_int_stack *s);
int							st_int_peek(t_int_stack *s);

struct						s_ptr_stack;
typedef struct s_ptr_stack	t_ptr_stack;

t_ptr_stack					*st_ptr_create(t_mem_mgr *m);
void						st_ptr_destroy(t_mem_mgr *m, t_ptr_stack *ps);
int							st_ptr_push(t_ptr_stack *ps, void *thing);
void						*st_ptr_pop(t_ptr_stack *ps);
void						*st_ptr_peek(t_ptr_stack *ps);

#endif