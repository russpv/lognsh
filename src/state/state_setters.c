#include "state_int.h"

void	set_parser(t_state *state, t_parser *p)
{
	state->current_parser = p;
}

void	set_lexer(t_state *state, t_lex *l)
{
	state->current_lexer = l;
}

void	set_command(t_state *s, t_cmd *c)
{
	s->current_cmd = c;
}

void	set_input(t_state *s, char *input)
{
	int	len;

	len = 0;
	if (!s)
		return ;
	s->input = input;
	if (NULL != input)
		len = ft_strnlen(input, MAX_INPUT_SZ);
	if (len == MAX_INPUT_SZ)
		return ;
	mem_add_mem(&s->mem_mgr.list, input, len + 1);
}

void	set_got_heredoc(t_state *s)
{
	if (NULL == s)
		return ;
	s->got_heredoc = true;
}
