/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   state_getters_env1.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dayeo <dayeo@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/28 08:56:51 by dayeo             #+#    #+#             */
/*   Updated: 2025/02/16 15:18:15 by dayeo            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "state_int.h"

/* Returns new heap array each call */
char	**get_envp(t_state *s)
{
	extern char	**environ;

	if (!s)
		return (NULL);
	if (!s->sh_env_list)
		s->sh_env_list = copy_envp(environ);
	return (lst_to_array(s->sh_env_list));
}

/* Passthrough getter to retrieve individual v in k:v */
char	*get_sh_env(t_state *s, const char *key)
{
	if (!s)
		return (NULL);
	if (!s->sh_env_list)
		return (NULL);
	return (env_getenv_value(key, s->sh_env_list));
}

/* Returns heap array that caller must free */
char	**get_sh_path(t_state *s)
{
	if (!s)
		return (NULL);
	return (env_getenv());
}
