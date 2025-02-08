#include "state_int.h"

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

t_state	*init_state(char **envp)
{
 	t_state *s = malloc(sizeof(struct s_global_state));
	if (s)
	{
		s->pwd = NULL; // TODO remove?
		s->current_exit_code = 0;
		s->error_code = 0;
		s->current_parser = NULL;
		s->current_lexer = NULL;
		s->current_cmd = NULL;
		s->input = NULL;
		s->envp = envp;
		s->destroy_command = NULL;
		s->destroy_lexer = NULL;
		s->destroy_parser = NULL;
	}
	return (s);
}

/* Destroys parser before lexer 
 * as parser wraps some lexer token content
 */
void	destroy_state(t_state *s)
{
	if (s->current_parser)
		s->destroy_parser(s->current_parser);
	if (s->current_lexer)
		s->destroy_lexer(s->current_lexer);
	if (s->current_cmd)
		s->destroy_command(s->current_cmd);
	if (s->input)
		free(s->input);
	free(s);
}

/* Destroys parser before lexer 
 * as parser wraps some lexer token content
 */
void	s_free_cmd_lex_parse(t_state *state)
{
	if (TESTFLAG == 0)
		free(state->input);
	state->input = NULL;
	if (state->current_parser)
	{
		state->destroy_parser(state->current_parser);
		state->current_parser = NULL;
	}
	if (state->current_lexer)
	{
		state->destroy_lexer(state->current_lexer);
		state->current_lexer = NULL;
	}
	if (state->current_cmd)
	{
		state->destroy_command(state->current_cmd);
		state->current_cmd = NULL;
	}
}

/* Destroys parser before lexer 
 * as parser wraps some lexer token content
 */
void	s_free_cmd(t_state *state)
{
	if (state->current_cmd)
	{
		state->destroy_command(state->current_cmd);
		state->current_cmd = NULL;
	}
}
