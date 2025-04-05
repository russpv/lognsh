/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   state_frees.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dayeo <dayeo@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/05 11:55:40 by dayeo             #+#    #+#             */
/*   Updated: 2025/04/05 11:56:03 by dayeo            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "state_int.h"

/* Destroys parser before lexer
 * as parser wraps some lexer token content
 */
void	s_free_cmd_lex_parse(t_state *state)
{
	t_mem_mgr	*m;

	m = get_mem(state);
	if (TESTFLAG == 0)
		m->dealloc(&m->list, state->input);
	state->input = NULL;
	if (state->current_parser)
	{
		state->destroy_parser(m, (void**)&state->current_parser);
		state->current_parser = NULL;
	}
	if (state->current_lexer)
	{
		state->destroy_lexer(m, (void**)&state->current_lexer);
		state->current_lexer = NULL;
		state->got_heredoc = false;
	}
	s_free_cmd(state);
	s_free_tmp(state);
}

/* Destroys current cmd
 */
void	s_free_cmd(t_state *state)
{
	if (state->current_cmd)
	{
		state->destroy_command(get_mem(state), (void**)&state->current_cmd);
		state->current_cmd = NULL;
		state->got_heredoc = false;
	}
	s_free_tmp(state);
}

extern void		destroy_token(t_mem_mgr *m, void **token);

// Note: req'd to release dup'd subword aggregation
void	s_free_tmp(t_state *s)
{
	t_mem_mgr	*m;

	m = get_mem(s);
	if (s->tmp_tok_lst)
	{
		ft_lstclear_tmp(m, &s->tmp_tok_lst, destroy_token);
		s->tmp_tok_lst = NULL;
	}
	if (s->tmp)
	{
		m->dealloc(&m->list, s->tmp);
		s->tmp = NULL;
	}
}
