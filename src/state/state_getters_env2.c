/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   state_getters_env2.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dayeo <dayeo@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/28 08:56:51 by dayeo             #+#    #+#             */
/*   Updated: 2025/02/16 15:18:15 by dayeo            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "state_int.h"

t_env	*get_sh_env_list(t_state *s)
{
	if (!s)
		return (NULL);
	return (s->sh_env_list);
}

t_env	**get_sh_env_list_add(t_state *s)
{
	if (!s)
		return (NULL);
	return (&s->sh_env_list);
}

char	*get_pwd(t_state *s)
{
	if (!s)
		return (NULL);
	return (s->pwd);
}
