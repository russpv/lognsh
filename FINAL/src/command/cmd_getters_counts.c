/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cmd_getters_counts.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rpeavey <rpeavey@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/10 16:28:27 by rpeavey           #+#    #+#             */
/*   Updated: 2025/04/10 16:28:28 by rpeavey          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "command_int.h"

int	c_get_cmdc(const t_cmd *c)
{
	if (!c)
		return (-1);
	return (c->curr_cmdc);
}

int	c_get_redc(const t_cmd *c)
{
	if (!c)
		return (-1);
	return (c->redc);
}
