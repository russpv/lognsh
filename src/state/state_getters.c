#include "state_int.h"

t_cmd	*get_cmd(t_state *s)
{
	if (!s)
		return (NULL);
	return (s->current_cmd);
}

char	*get_tmp(t_state *s)
{
	if (!s)
		return (NULL);
	return (s->tmp);
}

t_lex	*get_lexer(t_state *s)
{
	if (!s)
		return (NULL);
	return (s->current_lexer);
}

// Returns memory manager struct
t_mem_mgr *get_mem(t_state *s)
{
	if (!s)
		return (NULL);
	return (&s->mem_mgr);
}

char *get_input(t_state *s)
{
	if (!s)
		return (NULL);
	return (s->input);
}

char	*get_prompt(t_state *s)
{
	if (!s)
		return (NULL);
	if (!s->prompt)
		return (DFL_PROMPT);
	return (get_env_val(s, PROMPT_KEY));
}