/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_set.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dayeo <dayeo@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/09 09:04:50 by dayeo             #+#    #+#             */
/*   Updated: 2025/02/09 20:05:34 by dayeo            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "env_int.h"

// Set or update an environment variable
int		env_set_var(t_env **env, const char *key, const char *value)
{
	t_env	*tmp;
	t_env	*new_node;

	tmp = *env;
	while (tmp)
	{
		if (ft_strcmp(tmp->key, key) == 0)
		{
			free(tmp->value);
			tmp->value = ft_strdup(value);
			return (0);
		}
		tmp = tmp->next;
	}
    new_node = create_env_node(key, value);
	if (!new_node)
		return (-1);
	// if a key does not exist, add it as a new node
    add_env_node(env, new_node);
    return (0);
}

// remove an enviroment var
int		env_unset_var(t_env **env, const char *key)
{
	if (!env || !key)
		return (-1);

	remove_env_node(env, key);
	return (0);
}
