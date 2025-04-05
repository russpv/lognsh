/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cmd_utils_2.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dayeo <dayeo@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/05 18:30:52 by dayeo             #+#    #+#             */
/*   Updated: 2025/04/05 18:31:57 by dayeo            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "command.h"

void	dealloc_str(t_state *s, char *str_to_free)
{
	t_mem_mgr	*mem_mgr;

	mem_mgr = get_mem(s);
	mem_mgr->dealloc(&mem_mgr->list, str_to_free);
}
