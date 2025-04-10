/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   state_getters_misc.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rpeavey <rpeavey@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/10 16:34:54 by rpeavey           #+#    #+#             */
/*   Updated: 2025/04/10 16:34:55 by rpeavey          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "state_int.h"

char	*get_pwd(t_state *s)
{
	if (!s)
		return (NULL);
	return (s->pwd);
}

/* Returns the last exit code. */
int	*get_status(t_state *s)
{
	if (!s)
		return (NULL);
	return (&s->current_exit_code);
}

t_list	**get_tmp_tok_list(t_state *s)
{
	if (!s)
		return (NULL);
	return (&s->tmp_tok_lst);
}
