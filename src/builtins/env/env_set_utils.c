/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_set_utils.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dayeo <dayeo@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/09 08:59:52 by dayeo             #+#    #+#             */
/*   Updated: 2025/02/09 08:59:52 by dayeo            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "env_int.h"

void	env_set_next(t_env *node, t_env *next)
{
	if (!node)
		return ;
	node->next = next;
}

void	env_set_node_value(t_env *node, const char *value)
{
	char	*new_value;

	if (!node)
		return ;
	if (node->value)
		free(node->value);
	if (value)
		new_value = ft_strdup(value);
	else
		new_value = ft_strdup("");
	if (!new_value)
		return ;
	node->value = new_value;
}
