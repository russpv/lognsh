#include "state_int.h"

int	set_exit_status(t_state *state, int value)
{
	state->current_exit_code = value;
	return (0);
}

/* BUILD COMMAND
 * Use command.h methods
 */
/*
t_cmd	build_command(char *input)
{
	//TODO, use command.h methods
}

char	*get_env_var(t_state *s, const char *key)
{
	//TODO, use env.h methods
}

void	set_env_var(t_state *s, const char *key, const char *value)
{
	//TODO, use env.h methods
}

char	*search_path(const char *command)
{
	//TODO
	//wrapper required for COMMAND for parsing
}

char	**expand_glob(const char *pattern)
{
	//TODO
	//wrapper required for COMMAND for parsing
}
*/
void	set_error(t_state *state, int code)
{
	state->error_code = code;
}

void	s_free_cmd(t_state *state)
{
	free(state->input);
	state->input = NULL;
	state->destroy_parser(state->current_parser);
	state->destroy_lexer(state->current_lexer);
}

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

char *get_input(t_state *s)
{
	return (s->input);
}

void	register_parser_destroy(t_state *s, t_destroy_fn fn)
{
	s->destroy_parser = fn;
}

void	register_lexer_destroy(t_state *s, t_destroy_fn fn)
{
	s->destroy_lexer = fn;
}

