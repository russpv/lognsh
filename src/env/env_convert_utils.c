/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_convert_utils.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dayeo <dayeo@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/09 19:24:31 by dayeo             #+#    #+#             */
/*   Updated: 2025/02/09 20:29:21 by dayeo            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "env_int.h"
#include "env.h"

void	copy_key_value(char *dest, const char *key, const char *value)
{
	ft_strcpy(dest, key); // copy KEY
	ft_strcat(dest, "="); // add '='
	if (value)
		ft_strcat(dest, value);
}
char	*malloc_pair_string(const char *key, const char *value)
{
	size_t		key_len;
	size_t		value_len;
	size_t		total_len;
	char	*pair_space;

	key_len = ft_strlen(key);
	if (value)
		value_len = ft_strlen(value);
	else
		value_len = 0;
	total_len = key_len + 1 + value_len + 1; //"KEY=" + value +\0
	pair_space = malloc(total_len);
	if (!pair_space)
		return (NULL);
	return (pair_space);
}

char	**alloc_env_array(t_env *env_list)
{
	t_env	*tmp;
	int		count;
	char	**env_array;

	// count no. of env vars
	if (!env_list)
        return (NULL);    
    	count = get_env_list_size(env_list);
		
	// allocate an array of pointers
	env_array = malloc(sizeof(char *) * (size_t)(count + 1));
	if (!env_array)
		return (NULL);
	// allocate memory memory for key value pair
	tmp = env_list;
	count = 0;
	while (tmp)
	{
		env_array[count] = malloc_pair_string(tmp->key, tmp->value);
		//free array, if failure
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

void	free_env_array(char **env_array)
{
	int		i;

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
	

	// copy each env-value pair into allocated array
	while (tmp)
	{
		copy_key_value(env_array[count], tmp->key, tmp->value);
		count++;
		tmp = tmp->next;
	}
	return (env_array);
}
