#include "state_int.h"

char	*get_input(t_state *s)
{
	return (s->input);
}

t_cmd	*get_cmd(t_state *s)
{
	return (s->current_cmd);
}

/* Returns the last exit code. */
int	*get_status(t_state *s)
{
	return (&s->current_exit_code);
}

char	*get_tmp(t_state *s)
{
	return (s->tmp);
}

t_lex	*get_lexer(t_state *s)
{
	return (s->current_lexer);
}
