#include "mem.h"

t_mem_block	*mem_init_block(void)
{
	t_mem_block	*new;

	new = malloc(sizeof(struct s_mem_block));
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

void		mem_mgr_init(t_mem_mgr *m)
{
	m->f = myalloc;
	m->dealloc = myfree;
	mem_init(&m->list);
}

void	mem_free(t_mem_block *m)
{
	free(m->payload);
	free(m);
}

// Inserts at the front behind head
int	mem_add_mem(t_mem_node *head, void *alloc, size_t size)
{
	t_mem_block	*new;
	t_mem_node	*first;

	if (!head || !alloc || !size)
		return (-1);
	new = mem_init_block();
	if (new)
	{
		new->payload = alloc;
		new->size = size;
		first = head->next;
		if (first == head)
		{
			new->node.next = head;
			new->node.prev = head;
			head->next = &new->node;
			head->prev = &new->node;			
		}
		else
		{
			new->node.next = first;
			new->node.prev = head;
			head->next = &new->node;
			first->prev = &new->node;
		}	
		return (0);
	}
	return (ENOMEM);
}

t_mem_block	*get_mem_block_from_node(t_mem_node *node_ptr)
{
	if (!node_ptr)
		return (NULL);
	return (t_mem_block *)((char *)node_ptr - offsetof(t_mem_block, node));
}

// Frees all blocks in list. Avoids double free from manual frees.
void	mem_free_all(t_mem_node *head)
{
	t_mem_node	*node;
	t_mem_block	*block;

	if (!head)
		return ;
	node = head->next;
	while (node != head)
	{
		block = get_mem_block_from_node(node);
		node = node->next;
		if (false == block->is_freed)
			free(block->payload);
		free(block);
	}
}

t_mem_block	*mem_get_alloc(t_mem_node *head, void *alloc)
{
	t_mem_node	*node;
	t_mem_block	*block;

	if (!head || !alloc)
		return (NULL);
	node = head->next;
	while (node != head)
	{
		block = get_mem_block_from_node(node);
		if (!block)
			return (NULL);
		if (alloc == block->payload)
			return (block);
		node = node->next;
	}
	fprintf(stderr, "%s: block not found (%s) \n", __FUNCTION__, (char*)alloc);
	return (NULL);
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
			mem_free_all(head);
			perror("Memory allocation failed.\n");
			exit(ENOMEM);
		}
	}
	return (new);
}

// TODO add mem_rm_block(...);

void	myfree(t_mem_node *head, void *alloc)
{
	t_mem_block	*block;

	if (!head || !alloc)
		return ;
	block = mem_get_alloc(head, alloc);
	if (!block)
	{
		err("Memory corruption or double free detected! (maybe, WIP)\n");
		return ;
	}
	if (true == block->is_freed)
	{
		return (err("Double free detected.\n"));
	}
	block->is_freed = true;
	ft_memset(alloc, 0, block->size);
	free(alloc);
}

