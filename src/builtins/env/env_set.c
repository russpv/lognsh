/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_set.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rpeavey <rpeavey@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/10 16:26:09 by rpeavey           #+#    #+#             */
/*   Updated: 2025/04/10 16:26:10 by rpeavey          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "env_int.h"

void	env_set_node_next(t_env *node, t_env *next)
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

// Returns 0 if error.
// updates an existing environment variable's value
static int	_update_existing_var(t_mem_mgr *m, t_env *existing_key, \
				const char *value)
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

/* Returns the key's t_env node if present */
t_env	*env_find_node(t_env *env_list, const char *key)
{
	t_env	*current;

	if (!env_list || !key)
		return (NULL);
	current = env_list;
	while (current)
	{
		if (ft_strcmp(env_get_node_key(current), key) == 0)
			return (current);
		current = env_get_node_next(current);
	}
	return (NULL);
}

/* Updates or adds new key and value */
int	env_upsert_value(t_mem_mgr *m, t_env *env_list, const char *key,
	const char *value)
{
	t_env	*node;

	if (!env_list || !key)
		return (ERR_GENERAL);
	node = env_find_node(env_list, key);
	if (node)
	{
		dprint("%s: found key:%s\n", __FUNCTION__, key);
		if (!_update_existing_var(m, node, value))
			return (ERR_GENERAL);
	}
	else
	{
		dprint("%s: inserting key:%s\n", __FUNCTION__, key);
		node = create_env_node(m, key, value);
		if (!node)
			return (ERR_MEM);
		env_add_node(&env_list, node);
	}
	return (0);
}
