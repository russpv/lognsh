#include "state_int.h"

t_state	*init_state(char **envp)
{
	t_state	*s;

	if (!envp)
		return (NULL);
	s = malloc(sizeof(struct s_global_state));
	if (!s)
		return (NULL);
	s->pwd = NULL;
	s->current_exit_code = 0;
	s->error_code = 0;
	s->current_parser = NULL;
	s->current_lexer = NULL;
	s->current_cmd = NULL;
	s->input = NULL;
	s->destroy_command = NULL;
	s->destroy_lexer = NULL;
	s->destroy_parser = NULL;
	s->envp = envp;
	s->tmp = NULL;
	mem_mgr_init(&s->mem_mgr);
	mem_add_mem(&s->mem_mgr.list, s, sizeof(struct s_global_state));
	s->sh_env_list = copy_envp(envp);
	if (!s->sh_env_list)
		return (free(s), NULL);
	set_signal_handlers();
	return (s);
}

/* Destroys parser before lexer
 * as parser wraps some lexer token content
 */
void	destroy_state(t_state *s)
{
	if (s->current_parser)
		s->destroy_parser(s, s->current_parser);
	if (s->current_lexer)
		s->destroy_lexer(s, s->current_lexer);
	if (s->current_cmd)
		s->destroy_command(s, s->current_cmd);
	if (s->input)
		free(s->input);
	if (s->sh_env_list)
		env_free_list(s->sh_env_list);
	mem_free_all(&s->mem_mgr.list);
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
		state->destroy_parser(state, state->current_parser);
		state->current_parser = NULL;
	}
	if (state->current_lexer)
	{
		state->destroy_lexer(state, state->current_lexer);
		state->current_lexer = NULL;
	}
	if (state->current_cmd)
	{
		state->destroy_command(state, state->current_cmd);
		state->current_cmd = NULL;
	}
	s_free_tmp(state);
}

/* Destroys current cmd
 */
void	s_free_cmd(t_state *state)
{
	if (state->current_cmd)
	{
		state->destroy_command(state, state->current_cmd);
		state->current_cmd = NULL;
	}
	s_free_tmp(state);
}

// Note: req'd to release dup'd subword aggregation
void	s_free_tmp(t_state *s)
{
	t_mem_mgr *m;

	m = get_mem(s);
	if (s->tmp)
	{
		m->dealloc(&m->list, s->tmp);
		s->tmp = NULL;
	}
}
