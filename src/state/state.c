#include "state_int.h"

static inline void	_init_state_ptrs(t_state *s)
{
	s->current_parser = NULL;
	s->current_lexer = NULL;
	s->current_cmd = NULL;
	s->input = NULL;
	s->destroy_command = NULL;
	s->destroy_lexer = NULL;
	s->destroy_parser = NULL;
	s->tmp = NULL;
	s->pwd = NULL;
}

t_state	*init_state(char **envp)
{
	t_state	*s;

	if (!envp)
		return (NULL);
	s = malloc(sizeof(struct s_global_state));
	if (!s)
		return (NULL);
	s->current_exit_code = 0;
	s->error_code = 0;
	s->envp = envp;
	_init_state_ptrs(s);
	mem_mgr_init(&s->mem_mgr);
	mem_add_mem(&s->mem_mgr.list, s, sizeof(struct s_global_state));
	s->sh_env_list = copy_envp(&s->mem_mgr, envp);
	if (!s->sh_env_list)
		return (free(s), exit(ENOMEM), NULL);
	sig_set_handlers();
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
		env_free_list(get_mem(s), s->sh_env_list);
	mem_free_all(&s->mem_mgr.list);
}
