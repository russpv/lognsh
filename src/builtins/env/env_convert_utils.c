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

static char	*lst_join_env(char *key, char *val)
{
	char	*tmp;
	char	*res;

	if (!key)
		return (NULL);
	tmp = ft_strjoin(key, "=");
	if (!tmp)
		return (NULL);
	if (!val)
		return (tmp);
	res = ft_strjoin_free(tmp, val);
	if (!res)
		return (NULL);
	return (res);
}

char	**lst_to_array(t_env *env_list)
{
	char	**env_array;
	t_env	*tmp;
	int		i;

	if (!env_list)
		return (NULL);
	i = get_env_list_size(env_list);
	env_array = malloc(sizeof(char *) * (i + 1));
	if (!env_array)
		return (NULL);
	tmp = env_list;
	i = 0;
	while (tmp)
	{
		env_array[i] = lst_join_env(env_get_key(tmp), env_get_value(tmp));
		if (!env_array[i])
			return (free_env_array(env_array), NULL);
		i++;
		tmp = tmp->next;
	}
	env_array[i] = NULL;
	return (env_array);
}
