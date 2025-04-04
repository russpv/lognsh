/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   mem_utils_1.c                                        :+:      :+:    :+: */
/*                                                    +:+ +:+         +:+     */
/*   By: dayeo <dayeo@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/04 15:51:52 by dayeo             #+#    #+#             */
/*   Updated: 2025/04/04 15:52:06 by dayeo            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

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

void	mem_free(t_mem_block *m)
{
	free(m->payload);
	free(m);
}

t_mem_block	*get_mem_block_from_node(t_mem_node *node_ptr)
{
	if (!node_ptr)
		return (NULL);
	return ((t_mem_block *)((char *)node_ptr - offsetof(t_mem_block, node)));
}
