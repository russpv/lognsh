/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_get_utils.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rpeavey <rpeavey@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/09 08:59:52 by dayeo             #+#    #+#             */
/*   Updated: 2025/03/13 18:23:21 by rpeavey          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "env_int.h"

// this returns a system-managed string array. Do not modify!
char	**env_getpath(t_mem_mgr *m)
{
	char		**res;
	const char	*path_env = getenv("PATH");
	struct s_mem_utils utils;

	if (!path_env)
	{
		perror("getenv\n"); 
		return (NULL);
	}
	utils.head = &m->list;
	utils.f = m->f;
	utils.u = m->dealloc;
	res = ft_split_mem(&utils, path_env, ':');
	if (!res)
		exit_clean(&m->list, ENOMEM, __FUNCTION__, EMSG_MALLOC);
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

char	*env_get_key(t_env *node)
{
	if (!node)
		return (NULL);
	return (node->key);
}

char	*env_get_value(t_env *node)
{
	if (!node)
		return (NULL);
	return (node->value);
}

t_env	*env_get_next(t_env *node)
{
	if (!node)
		return (NULL);
	return (node->next);
}
