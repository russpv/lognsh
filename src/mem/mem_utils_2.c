/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   mem_utils_2.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dayeo <dayeo@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/04 15:58:22 by dayeo             #+#    #+#             */
/*   Updated: 2025/04/04 15:58:36 by dayeo            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "mem.h"

int	mem_insert_block(t_mem_node *head, t_mem_block *new)
{
	t_mem_node	*first;

	if (!head || !new)
		return (-1);
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

int	mem_add_mem(t_mem_node *head, void *alloc, size_t size)
{
	t_mem_block	*new;

	if (!head || !alloc || !size)
		return (-1);
	new = mem_init_block();
	if (!new)
		return (ENOMEM);
	new->payload = alloc;
	new->size = size;
	if (mem_insert_block(head, new) != 0)
	{
		mem_free(new);
		return (-1);
	}
	return (0);
}

// Inserts at the front behind head
/*int	mem_add_mem(t_mem_node *head, void *alloc, size_t size)
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
}*/

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
	debug_print("%s: block not found (%s) \n", __FUNCTION__, (char *)alloc);
	return (NULL);
}
