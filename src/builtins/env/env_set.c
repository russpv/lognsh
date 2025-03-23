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

#define EMSG_BADMALLOC "memory allocation failed.\n"

// Returns 0 if error.
// updates an existing environment variable's value
int	update_existing_var(t_mem_mgr *m, t_env *existing_key, const char *value)
{
	char	*new_value;

	new_value = NULL;
	if (!existing_key)
		return (0);
	if (value)
		new_value = ft_strdup(value);
	else
		new_value = ft_strdup("");
	if (!new_value)
	{
		exit_clean(&m->list, ENOMEM, __FUNCTION__, EMSG_MALLOC);
	}
	env_set_node_value(m, existing_key, new_value);
	free(new_value);
	return (1);
}

t_env	*find_env_key(t_env *env_list, const char *key)
{
	t_env	*current;

	if (!env_list || !key)
		return (NULL);
	current = env_list;
	while (current)
	{
		if (ft_strcmp(env_get_key(current), key) == 0)
			return (current);
		current = env_get_next(current);
	}
	return (NULL);
}


int	env_upsert_value(t_mem_mgr *m, t_env **env_list, const char *key,
	const char *value)
{
t_env	*node;

if (!env_list || !key)
	return (1);
node = find_env_key(*env_list, key);
if (node)
{
	debug_print("%s: found key:%s\n", __FUNCTION__, key);
	if (!update_existing_var(m, node, value))
		return (1);
}
else
{
	debug_print("%s: inserting key:%s\n", __FUNCTION__, key);
	node = create_env_node(m, key, value);
	if (!node)
		return (1);
	env_add_node(env_list, node);
}
return (0);
}
