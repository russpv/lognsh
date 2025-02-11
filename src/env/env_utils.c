/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_utils.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dayeo <dayeo@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/09 09:06:55 by dayeo             #+#    #+#             */
/*   Updated: 2025/02/09 19:58:19 by dayeo            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "env_int.h"

// helper function for copy_envp() to get env size
int	get_env_size(char **envp)
{
	int	size;

	size = 0;
	while (envp[size])
		size++;
	return (size);
}

// get list size of env in linked list
int	get_env_list_size(t_env *env_list)
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

// free all env variables
void	env_free(t_env *env)
{
	t_env	*tmp;

	while (env)
	{
		tmp = env;
		env = env->next;
		free(tmp->key);
		if (tmp->value)
			free(tmp->value);
		free(tmp);
	}
}

// print all env variables
void	env_print(t_env *env)
{
	while (env)
	{
		printf("%s=", env->key);
		if (env->value)
			printf("%s\n", env->value);
		else
			printf("\n");
		env = env->next;
	}
}
