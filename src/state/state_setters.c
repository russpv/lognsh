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

void	set_tmp(t_state *s, char *str)
{
	if (NULL != s->tmp)
		get_mem(s)->dealloc(&get_mem(s)->list, s->tmp);
	s->tmp = ft_strdup_tmp(get_mem(s), str);
}
