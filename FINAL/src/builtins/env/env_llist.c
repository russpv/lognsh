/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_llist.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rpeavey <rpeavey@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/10 16:25:42 by rpeavey           #+#    #+#             */
/*   Updated: 2025/04/10 16:25:43 by rpeavey          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "env_int.h"

// Adds node to back of llist
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

// Removes llist node based on key
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

// Create new environment k,v llist node
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

// extracts key and value from raw env var string "key=value"
// key must be non-empty
static int	_extract_key_value(const char *env_str, char key[], char value[])
{
	char	*equal_pos;
	size_t	valuelen;

	equal_pos = ft_strchr(env_str, '=');
	if (!equal_pos)
		return (-1);
	if (equal_pos == env_str)
		return (-1);
	if (0 == ft_strscpy(key, env_str, (size_t)(equal_pos - env_str) + 1))
		return (-1);
	valuelen = ft_strnlen(equal_pos + 1, MAX_INPUT_SZ);
	if (valuelen)
		if (0 == ft_strscpy(value, equal_pos + 1, valuelen + 1))
			return (-1);
	return (0);
}

// Returns deep copy of envp array as t_env* linked list
t_env	*copy_envp(t_mem_mgr *m, char **envp)
{
	t_env	*env_list;
	char	key[MAX_NAME_LEN];
	char	value[MAX_INPUT_SZ];
	int		i;
	t_env	*new_node;

	env_list = NULL;
	i = 0;
	dprint(_MOD_ ": copying envp\n");
	while (envp[i] != NULL)
	{
		ft_memset(key, 0, sizeof(key));
		ft_memset(value, 0, sizeof(value));
		if (_extract_key_value(envp[i], key, value) == 0)
		{
			new_node = create_env_node(m, key, value);
			if (!new_node)
				return (NULL);
			env_add_node(&env_list, new_node);
		}
		i++;
	}
	return (env_list);
}
