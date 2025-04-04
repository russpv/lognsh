/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   mem.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dayeo <dayeo@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/04 15:50:03 by dayeo             #+#    #+#             */
/*   Updated: 2025/04/04 15:50:05 by dayeo            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "mem.h"

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
// TODO add mem_rm_block(...);
