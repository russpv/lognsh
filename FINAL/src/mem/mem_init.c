/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   mem_init.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rpeavey <rpeavey@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/10 16:33:15 by rpeavey           #+#    #+#             */
/*   Updated: 2025/04/13 21:32:10 by rpeavey          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "mem.h"
#include <stdint.h>

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
	if ((uintptr_t)alloc == 0xAAAAAAAAAAAAAACA)
	{
		dprintf(2, "BUG: Trying to free poisoned pointer! 💥\n");
		abort(); 
	}
	if (!mem_get_alloc(head, alloc)) {
		dprintf(2, "BUG: myfree() called on unmanaged or already freed pointer: %p\n", alloc);
		return; // or abort() if you're strict
	}
	block = mem_get_alloc(head, alloc);
	if (!block) {
		fprintf(stderr, "myfree: tried to free unknown pointer %p\n", alloc);
		return (err("Memory corruption or double free detected!\n"));
	}
	if (true == block->is_freed)
		return (err("Double free detected.\n"));
	block->is_freed = true;
	ft_memset(alloc, 0, block->size);
	ft_memset(block, 0xAA, sizeof(*block)); // Catch any corruption.
	free(alloc);
}
