/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   state_getters.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dayeo <dayeo@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/05 11:56:25 by dayeo             #+#    #+#             */
/*   Updated: 2025/04/05 11:56:36 by dayeo            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "state_int.h"

t_cmd	*get_cmd(t_state *s)
{
	if (!s)
		return (NULL);
	return (s->current_cmd);
}

char	*get_tmp(t_state *s)
{
	if (!s)
		return (NULL);
	return (s->tmp);
}

t_lex	*get_lexer(t_state *s)
{
	if (!s)
		return (NULL);
	return (s->current_lexer);
}

// Returns memory manager struct
t_mem_mgr	*get_mem(t_state *s)
{
	if (!s)
		return (NULL);
	return (&s->mem_mgr);
}

char	*get_input(t_state *s)
{
	if (!s)
		return (NULL);
	return (s->input);
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
