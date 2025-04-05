/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   state_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dayeo <dayeo@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/05 11:57:15 by dayeo             #+#    #+#             */
/*   Updated: 2025/04/05 11:57:16 by dayeo            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "state_int.h"

/* Returns true if state's sh_env_list is non-NULL */
bool	has_sh_envp(t_state *s)
{
	if (!s)
		return (false);
	return (NULL != s->sh_env_list);
}
