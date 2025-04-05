/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   state_getters_misc.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dayeo <dayeo@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/05 11:56:19 by dayeo             #+#    #+#             */
/*   Updated: 2025/04/05 11:56:20 by dayeo            ###   ########.fr       */
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
