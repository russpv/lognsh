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

void	set_tmp(t_state *s, char *str)
{
	if (s->tmp)
		myfree(&get_mem(s)->list, s->tmp);
	s->tmp = ft_strdup_tmp(get_mem(s), str);
}

/* Exits without leaks. */
void	exit_clean(t_mem_node *head, int exit_status, const char *caller, const char *errmsg)
{
	if (errmsg)
		print_custom_err(caller, errmsg);
	mem_free_all(head);
	if (exit_status)
		exit(exit_status);
	exit(ENOMEM);
}
