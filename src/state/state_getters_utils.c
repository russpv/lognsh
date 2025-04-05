/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   state_getters_utils.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dayeo <dayeo@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/05 20:34:22 by dayeo             #+#    #+#             */
/*   Updated: 2025/04/05 20:34:38 by dayeo            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "state_int.h"

char	*get_tmp(t_state *s)
{
	if (!s)
		return (NULL);
	return (s->tmp);
}

char	*get_prompt(t_state *s)
{
	if (!s)
		return (NULL);
	if (!s->prompt)
		return (DFL_PROMPT);
	return (get_env_val(s, PROMPT_KEY));
}

bool	get_heredoc(t_state *s)
{
	if (!s)
		return (false);
	if (!get_lexer(s))
		return (false);
	return (s->got_heredoc);
}

int	get_tmp_flag(t_state *s)
{
	if (!s)
		return (INT_MAX);
	return (s->tmp_flag);
}
