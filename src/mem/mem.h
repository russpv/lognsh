/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   mem.h                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dayeo <dayeo@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/04 15:49:11 by dayeo             #+#    #+#             */
/*   Updated: 2025/04/04 15:49:16 by dayeo            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MEM_H
# define MEM_H

# include "../data_structures/i_llist.h"
# include "../state/state.h"
# include <errno.h>

//Core Memory Management API: mem.c 
void		mem_mgr_init(t_mem_mgr *m);
void		mem_struct_init(t_mem_mgr *m, struct s_mem_utils *s);
/* t_alloc_fn and t_dealloc_fn */
void		*myalloc(t_mem_node *head, size_t size);
void		myfree(t_mem_node *head, void *alloc);
void		mem_free_all(t_mem_node *head);

//Low-Level Block and List Utils: mem_utils_1.c 
t_mem_block	*mem_init_block(void);
void		mem_init(t_mem_node *head);
void		mem_free(t_mem_block *m);
t_mem_block	*get_mem_block_from_node(t_mem_node *node_ptr);

//Allocation and Lookup Helpers: mem_utils_2.c
int			mem_insert_block(t_mem_node *head, t_mem_block *new);
int			mem_add_mem(t_mem_node *head, void *alloc, size_t size);
t_mem_block	*mem_get_alloc(t_mem_node *head, void *alloc);

extern void	exit_clean(t_mem_node *head, int exit_status, const char *caller,
				const char *errmsg);

#endif
