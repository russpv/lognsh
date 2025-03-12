#ifndef MEM_H
# define MEM_H

# include "../data_structures/i_llist.h"
# include <errno.h>

t_mem_block	*mem_init_block(void);
void		mem_init(t_mem_node *head);
void		mem_mgr_init(t_mem_mgr *m);

void		mem_free(t_mem_block *m);
int			mem_add_mem(t_mem_node *head, void *alloc, size_t size);
void		mem_free_all(t_mem_node *head);
t_mem_block	*mem_get_alloc(t_mem_node *head, void *alloc);

/* t_alloc_fn and t_dealloc_fn */
void		*myalloc(t_mem_node *head, size_t size);
void		myfree(t_mem_node *head, void *alloc);

#endif
