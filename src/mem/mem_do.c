#include "mem.h"

static void	_assign(t_mem_block *new, t_mem_node *first, t_mem_node *head)
{
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
		_assign(new, first, head);
		return (0);
	}
	return (ENOMEM);
}

t_mem_block	*get_mem_block_from_node(t_mem_node *node_ptr)
{
	if (!node_ptr)
		return (NULL);
	return ((t_mem_block *)((char *)node_ptr - offsetof(t_mem_block, node)));
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
	dprint("%s: block not found (%s) \n", __FUNCTION__, (char *)alloc);
	return (NULL);
}
