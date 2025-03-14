#include "state_int.h"

/* Destroys parser before lexer
 * as parser wraps some lexer token content
 */
void	s_free_cmd_lex_parse(t_state *state)
{
	if (TESTFLAG == 0)
		free(state->input);
	state->input = NULL;
	if (state->current_parser)
	{
		state->destroy_parser(state, state->current_parser);
		state->current_parser = NULL;
	}
	if (state->current_lexer)
	{
		state->destroy_lexer(state, state->current_lexer);
		state->current_lexer = NULL;
	}
	if (state->current_cmd)
	{
		state->destroy_command(state, state->current_cmd);
		state->current_cmd = NULL;
	}
	s_free_tmp(state);
}

/* Destroys current cmd
 */
void	s_free_cmd(t_state *state)
{
	if (state->current_cmd)
	{
		state->destroy_command(state, state->current_cmd);
		state->current_cmd = NULL;
	}
	s_free_tmp(state);
}

// Note: req'd to release dup'd subword aggregation
void	s_free_tmp(t_state *s)
{
	t_mem_mgr *m;

	m = get_mem(s);
	if (s->tmp)
	{
		m->dealloc(&m->list, s->tmp);
		s->tmp = NULL;
	}
}
