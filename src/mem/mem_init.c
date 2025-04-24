
#include "mem.h"

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

void	myfree(t_mem_node *head, void *alloc)
{
	t_mem_block	*block;

	if (!head || !alloc)
		return ;
	block = mem_get_alloc(head, alloc);
	if (!block)
		return (err("Memory corruption or double free detected!\n"));
	if (true == block->is_freed)
		return (err("Double free detected.\n"));
	block->is_freed = true;
	ft_memset(alloc, 0, block->size);
	free(alloc);
}
