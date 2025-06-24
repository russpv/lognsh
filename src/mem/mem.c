#include "mem.h"

t_mem_block	*mem_init_block(void)
{
	t_mem_block	*new;

	new = (t_mem_block *)malloc(sizeof(struct s_mem_block));
	if (new)
	{
		new->node.next = NULL;
		new->node.prev = NULL;
		new->payload = NULL;
		new->is_freed = false;
		new->size = 0;
	}
	return (new);
}

void	mem_init(t_mem_node *head)
{
	head->next = head;
	head->prev = head;
}

void	mem_mgr_init(t_mem_mgr *m)
{
	m->f = myalloc;
	m->dealloc = myfree;
	mem_init(&m->list);
}

void	mem_struct_init(t_mem_mgr *mgr, struct s_mem_utils *m)
{
	m->f = mgr->f;
	m->u = mgr->dealloc;
	m->head = &mgr->list;
}

void	mem_free(t_mem_block *m)
{
	free(m->payload);
	free(m);
}

void	*myalloc(t_mem_node *head, size_t size)
{
	void	*new;

	if (size < 1)
		return (NULL);
	new = malloc(size);
	if (new)
	{
		if (0 != mem_add_mem(head, new, size))
		{
			perror(MSG_FAIL);
			mem_free_all(head);
			exit(ENOMEM);
		}
	}
	else
	{
		perror(MSG_FAIL);
		mem_free_all(head);
		exit(ENOMEM);
	}
	return (new);
}

// Frees only the block->payload.
void	myfree(t_mem_node *head, void *alloc)
{
	t_mem_block	*block;

	if (!head || !alloc)
		return ;
	if ((uintptr_t)alloc == 0xAAAAAAAAAAAAAAAA)
	{
		dprintf(2, "myfree: Trying to free poisoned pointer! 💥\n");
		abort(); 
	}
	if (!mem_get_alloc(head, alloc)) {
		dprintf(2, "myfree: called on unmanaged or already freed pointer: %p\n", alloc);
		return ;
	}
	block = mem_get_alloc(head, alloc);
	if (!block) {
		fprintf(stderr, "myfree: tried to free unknown pointer %p\n", alloc);
		return ;
	}
	if (true == block->is_freed)
		return (err("Double free detected.\n"));
	block->is_freed = true;
	ft_memset(alloc, 0xAA, block->size);
	free(alloc);
}
