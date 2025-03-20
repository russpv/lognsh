/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_set_utils.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rpeavey <rpeavey@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/09 08:59:52 by dayeo             #+#    #+#             */
/*   Updated: 2025/03/12 18:47:05 by rpeavey          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "env_int.h"

void	env_set_next(t_env *node, t_env *next)
{
	if (!node)
		return ;
	node->next = next;
}

// Copies value into env llist
void	env_set_node_value(t_mem_mgr *m, t_env *node, const char *value)
{
	char	*new_value;

	if (!node)
		return ;
	if (node->value)
		m->dealloc(&m->list, node->value);
	if (value)
		new_value = ft_strdup_tmp(m, value);
	else
		new_value = ft_strdup_tmp(m, "");
	if (!new_value)
		return ;
	node->value = new_value;
}
