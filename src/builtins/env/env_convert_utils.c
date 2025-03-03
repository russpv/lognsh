/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_convert_utils.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dayeo <dayeo@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/09 19:24:31 by dayeo             #+#    #+#             */
/*   Updated: 2025/02/25 20:19:39 by dayeo            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "env.h"
#include "env_int.h"

// get list size of env in linked list
static int	get_env_list_size(t_env *env_list)
{
	int	count;

	count = 0;
	while (env_list)
	{
		count++;
		env_list = env_list->next;
	}
	return (count);
}

static char	*malloc_pair_string(const char *key, const char *value)
{
	size_t	key_len;
	size_t	value_len;
	size_t	total_len;
	char	*pair_space;

	key_len = ft_strlen(key);
	if (value)
		value_len = ft_strlen(value);
	else
		value_len = 0;
	total_len = key_len + 1 + value_len + 1;
	pair_space = malloc(total_len);
	if (!pair_space)
		return (NULL);
	return (pair_space);
}

static void	free_env_array(char **env_array)
{
	int	i;

	i = 0;
	if (!env_array)
		return ;
	while (env_array[i])
	{
		free(env_array[i]);
		i++;
	}
	free(env_array);
}

static char	**alloc_env_array(t_env *env_list)
{
	t_env	*tmp;
	int		count;
	char	**env_array;

	if (!env_list)
		return (NULL);
	count = get_env_list_size(env_list);
	env_array = malloc(sizeof(char *) * (size_t)(count + 1));
	if (!env_array)
		return (NULL);
	tmp = env_list;
	count = 0;
	while (tmp)
	{
		env_array[count] = malloc_pair_string(tmp->key, tmp->value);
		if (!env_array[count])
		{
			free_env_array(env_array);
			return (NULL);
		}
		count++;
		tmp = tmp->next;
	}
	env_array[count] = NULL;
	return (env_array);
}

/* Returns heap array */
char	**lst_to_array(t_env *env_list)
{
	char	**env_array;
	t_env	*tmp;
	int		count;

	count = 0;
	tmp = env_list;
	if (!env_list)
		return (NULL);
	env_array = alloc_env_array(env_list);
	if (!env_array)
		return (NULL);
	while (tmp)
	{
		ft_strcpy(env_array[count], env_get_key(tmp));
		ft_strchr(env_array[count], '=');
		if (env_get_value(tmp))
			ft_strcat(env_array[count], env_get_value(tmp));
		count++;
		tmp = tmp->next;
	}
	return (env_array);
}
