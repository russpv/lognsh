/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cmd_getters_args.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rpeavey <rpeavey@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/10 16:28:20 by rpeavey           #+#    #+#             */
/*   Updated: 2025/04/10 16:28:21 by rpeavey          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "command_int.h"

int	c_get_argvc(const t_cmd *c)
{
	if (!c)
		return (-1);
	return (c->argvc);
}

char	**c_get_argv(t_cmd *c)
{
	return (c->argv);
}
