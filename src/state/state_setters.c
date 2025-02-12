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
	s->input = input;
}
void	set_command(t_state *s, t_cmd *c)
{
	s->current_cmd = c;
}

int	set_exit_status(t_state *state, int value)
{
	state->current_exit_code = value;
	return (0);
}
