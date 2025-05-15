/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   state_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rpeavey <rpeavey@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/10 16:35:02 by rpeavey           #+#    #+#             */
/*   Updated: 2025/04/10 16:35:03 by rpeavey          ###   ########.fr       */
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
