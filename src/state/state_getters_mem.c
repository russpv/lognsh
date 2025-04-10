/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   state_getters_mem.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rpeavey <rpeavey@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/10 16:34:53 by rpeavey           #+#    #+#             */
/*   Updated: 2025/04/10 16:34:54 by rpeavey          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "state_int.h"

// Returns memory manager struct
t_mem_mgr	*get_mem(t_state *s)
{
	if (!s)
		return (NULL);
	return (&s->mem_mgr);
}

int	get_tmp_flag(t_state *s)
{
	if (!s)
		return (INT_MAX);
	return (s->tmp_flag);
}

char	*get_tmp(t_state *s)
{
	if (!s)
		return (NULL);
	return (s->tmp);
}
