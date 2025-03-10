#ifndef MEM_MGR_H
# define MEM_MGR_H

# include <errno.h>
# include <stdbool.h>
# include <stddef.h>
# include <stdlib.h>

typedef struct s_mem_node
{
	struct s_mem_node	*next;
	struct s_mem_node	*prev;
}						t_mem_node;

typedef struct s_mem_block
{
	t_mem_node			node;
	size_t				size;
	bool				is_freed;
	void				*payload;
}						t_mem_block;

t_mem_block				*mem_init_block(void);
void					mem_init(t_mem_node *head);

void					mem_free(t_mem_block *m);
int						mem_add_mem(t_mem_node *head, void *alloc, size_t size);
void					mem_free_all(t_mem_node *head);
t_mem_block				*mem_get_alloc(t_mem_node *head, void *alloc);

#endif