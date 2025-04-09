#include "state_int.h"

t_cmd	*get_cmd(t_state *s)
{
	if (!s)
		return (NULL);
	return (s->current_cmd);
}

t_lex	*get_lexer(t_state *s)
{
	if (!s)
		return (NULL);
	return (s->current_lexer);
}

bool	get_heredoc(t_state *s)
{
	if (!s)
		return (false);
	if (!get_lexer(s))
		return (false);
	return (s->got_heredoc);
}

char	*get_input(t_state *s)
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
