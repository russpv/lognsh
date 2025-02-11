/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dayeo <dayeo@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/09 09:00:07 by dayeo             #+#    #+#             */
/*   Updated: 2025/02/09 20:09:35 by dayeo            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "env_int.h"

// Create new environment variable node
t_env	*create_env_node(const char *key, const char *value)
{
	t_env	*new_node;

	new_node = malloc(sizeof(t_env));
	if (!new_node)
		return (NULL);

	new_node->key = ft_strdup(key);
	if (!new_node->key)
	{
		free(new_node);
		return (NULL);
	}
	if (value)
		new_node->value = ft_strdup(value);
	else
		new_node->value = NULL;
	new_node->next = NULL;
	return (new_node);
}

void	add_env_node(t_env **env, t_env *new_node)
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
		tmp =tmp->next;
	tmp->next = new_node;
}

void	remove_env_node(t_env **env, const char *key)
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
	if (tmp == NULL) //  key not found
		return ;
	if (prev == NULL) // removing head
		*env = tmp->next;
	else
		prev->next = tmp->next;
	free(tmp->key);
	if (tmp->value)
		free(tmp->value);
	free(tmp);
}
// helper function for copy_envp() - extracts key and value from env var
int	extract_key_value(const char *env_str, char **key, char **value)
{
	char	*equal_pos;

	equal_pos = ft_strchr(env_str, '=');
	if (!equal_pos)
		return (-1);
	*key = ft_strndup(env_str, (size_t)(equal_pos - env_str));
	if (!*key)
		return (-1);
	*value = ft_strdup(equal_pos + 1);
	if (!*value)
	{
		free(*key);
		return (-1);
	}
	return (0);
}


// copy env vars from envp array into a linked list
t_env	*copy_envp(char **envp)
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
		if (extract_key_value(envp[i], &key, &value) == 0)
		{
			new_node = create_env_node(key, value);
			if (!new_node)
			{
				free(key);
				free(value);
				free_env_list(env_list);
				return (NULL);
			}
			add_env_node(&env_list, new_node);
			free(key);
			free(value);
		}
		i++;
	}
	return (env_list);
}





/* returns new paths array 
 * getenv does not return heap memory */




/*
get_env_var(const char *key)
{
	//TODO
}

// add key value pair to ENV
set_env_var(char *key, char *value)
{
	//TODO
}

char	*search_path(const char *command)
{
	//TODO
}

char	**expand_glob(const char *pattern)
{
	//TODO
}

ht	*build_ht(void)
{
	//TODO
}
*/
