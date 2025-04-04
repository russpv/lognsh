#include "state_int.h"

t_lex_fns	*get_lex_fns(t_state *s)
{
	if (!s)
		return (NULL);
	return (s->lex_fns);
}

t_cmd_fns	*get_cmd_fns(t_state *s)
{
	if (!s)
		return (NULL);
	return (s->cmd_fns);
}

t_parse_fns	*get_parse_fns(t_state *s)
{
	if (!s)
		return (NULL);
	return (s->parse_fns);
}
