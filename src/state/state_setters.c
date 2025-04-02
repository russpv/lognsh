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
	int len;

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

void	set_tmp(t_state *s, char *str)
{
	if (NULL != s->tmp)
		get_mem(s)->dealloc(&get_mem(s)->list, s->tmp);
	s->tmp = ft_strdup_tmp(get_mem(s), str);
}

void	set_got_heredoc(t_state *s)
{
	if (NULL == s)
		return ;
	s->got_heredoc = true;
}

void	set_tmp_flag(t_state *s, int val)
{
	if (NULL == s)
		return ;
	s->tmp_flag = val;
}
