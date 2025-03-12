/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_llist.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rpeavey <rpeavey@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/09 09:00:07 by dayeo             #+#    #+#             */
/*   Updated: 2025/03/12 18:49:03 by rpeavey          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "env_int.h"

// Create new environment variable node
t_env	*create_env_node(t_mem_mgr *m, const char *key, const char *value)
{
	t_env	*new_node;

	new_node = m->f(&m->list, sizeof(t_env));
	if (!new_node)
		exit_clean(&m->list, ENOMEM, __FUNCTION__, "Malloc");
	new_node->key = ft_strdup_tmp(m, key);
	if (!new_node->key)
		exit_clean(&m->list, ENOMEM, __FUNCTION__, "Malloc");
	if (value)
		new_node->value = ft_strdup_tmp(m, value);
	if (NULL == new_node->value)
		exit_clean(&m->list, ENOMEM, __FUNCTION__, "Malloc");
	new_node->next = NULL;
	return (new_node);
}

void	env_add_node(t_env **env, t_env *new_node)
{
	t_env	*tmp;

	if (!env || !new_node)
		return ;
	if (*env == NULL)
	{
		*env = new_node;
		return ;
	}
	tmp = *env;
	while (tmp->next != NULL)
		tmp = tmp->next;
	tmp->next = new_node;
}

void	env_remove_node(t_mem_mgr *m, t_env **env, const char *key)
{
	t_env	*tmp;
	t_env	*prev;

	if (!env || !*env || !key)
		return ;
	tmp = *env;
	prev = NULL;
	while (tmp != NULL && ft_strcmp(tmp->key, key) != 0)
	{
		prev = tmp;
		tmp = tmp->next;
	}
	if (tmp == NULL)
		return ;
	if (prev == NULL)
		*env = tmp->next;
	else
		prev->next = tmp->next;
	m->dealloc(&m->list, tmp->key);
	if (tmp->value)
		m->dealloc(&m->list, tmp->value);
	m->dealloc(&m->list, tmp);
}

// helper function for copy_envp() - extracts key and value from env var
static int	_extract_key_value(t_mem_mgr *m, const char *env_str, char **key, char **value)
{
	char	*equal_pos;

	equal_pos = ft_strchr(env_str, '=');
	if (!equal_pos)
		return (-1);
	*key = ft_strndup(env_str, (size_t)(equal_pos - env_str)); //TODO fork
	if (!*key)
		return (-1);
	*value = ft_strdup_tmp(m, equal_pos + 1);
	if (!*value)
		exit_clean(&m->list, ENOMEM, __FUNCTION__, "Malloc");
	return (0);
}

// copy env vars from envp array into a linked list
t_env	*copy_envp(t_mem_mgr *m, char **envp)
{
	t_env	*env_list;
	char	*key;
	char	*value;
	int		i;
	t_env	*new_node;

	env_list = NULL;
	i = 0;
	while (envp[i] != NULL)
	{
		if (_extract_key_value(m, envp[i], &key, &value) == 0)
		{
			new_node = create_env_node(m, key, value);
			if (!new_node)
				return (NULL);
			env_add_node(&env_list, new_node);
			m->dealloc(&m->list, key);
			m->dealloc(&m->list, value);
		}
		i++;
	}
	return (env_list);
}
