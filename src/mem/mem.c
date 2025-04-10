/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   mem.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rpeavey <rpeavey@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/10 16:33:17 by rpeavey           #+#    #+#             */
/*   Updated: 2025/04/10 16:33:18 by rpeavey          ###   ########.fr       */
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
