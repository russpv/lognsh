/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_get.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dayeo <dayeo@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/09 08:59:52 by dayeo             #+#    #+#             */
/*   Updated: 2025/02/09 08:59:52 by dayeo            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "env_int.h"

// this returns a system-managed string. Do not modify!
char	**env_getenv(void)
{
	char		**res;
	const char	*path_env = getenv("PATH");

	if (!path_env)
	{
		perror("getenv error\n");
		return (NULL);
	}
	res = ft_split(path_env, ':');
	if (!res)
	{
		perror("ft_split error\n");
		return (NULL);
	}
	return (res);
}

// looks for an environment variable in the linked list and returns its value
char	*env_getenv_value(const char *key, t_env *env)
{
	if (!key || !env)
		return (NULL);
	while (env)
	{
		if (ft_strcmp(env->key, key) == 0)
			return (env->value);
		env = env->next;
	}
	return (NULL);
}
