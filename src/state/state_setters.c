/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   state_setters.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dayeo <dayeo@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/05 11:57:09 by dayeo             #+#    #+#             */
/*   Updated: 2025/04/05 11:57:10 by dayeo            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "state_int.h"

void	set_parser(t_state *state, t_parser *p)
{
	state->current_parser = p;
}

void	set_lexer(t_state *state, t_lex *l)
{
	state->current_lexer = l;
}

void	set_input(t_state *s, char *input)
{
	int	len;

	len = 0;
	if (!s)
		return ;
	s->input = input;
	if (NULL != input)
		len = ft_strnlen(input, MAX_RAW_INPUT_LEN + 1);
	if (len == MAX_RAW_INPUT_LEN + 1)
		return ;
	mem_add_mem(&s->mem_mgr.list, input, len + 1);
}

void	set_command(t_state *s, t_cmd *c)
{
	s->current_cmd = c;
}
