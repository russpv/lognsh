/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cmd_getters_args.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dayeo <dayeo@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/05 09:26:19 by dayeo             #+#    #+#             */
/*   Updated: 2025/04/05 09:26:32 by dayeo            ###   ########.fr       */
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
