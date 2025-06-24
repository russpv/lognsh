#include "state_int.h"

/* Destroys parser before lexer
 * as parser wraps some lexer token content
 */
void	s_free_cmd_lex_parse(t_state *state)
{
	t_mem_mgr	*m;

	m = get_mem(state);
	if (TESTFLAG == 0)
		m->dealloc(&m->list, state->input);
	state->input = NULL;
	if (state->current_parser)
	{
		state->destroy_parser(m, (void **)&state->current_parser);
		state->current_parser = NULL;
	}
	if (state->current_lexer)
	{
		state->destroy_lexer(m, (void **)&state->current_lexer);
		state->current_lexer = NULL;
		state->got_heredoc = false;
	}
	s_free_cmd(state);
	s_free_tmp(state);
}

/* Destroys current cmd
 */
void	s_free_cmd(t_state *state)
{
	if (state->current_cmd)
	{
		state->destroy_command(get_mem(state), (void **)&state->current_cmd);
		state->current_cmd = NULL;
		state->got_heredoc = false;
	}
	state->error_code = 0;
	s_free_tmp(state);
}

extern void	destroy_token(t_mem_mgr *m, void **token);

// Note: req'd to release dup'd subword aggregation
void	s_free_tmp(t_state *s)
{
	t_mem_mgr	*m;

	m = get_mem(s);
	if (s->tmp_tok_lst)
	{
		ft_lstclear_tmp(m, &s->tmp_tok_lst, destroy_token);
		s->tmp_tok_lst = NULL;
	}
	if (s->tmp)
	{
		m->dealloc(&m->list, s->tmp);
		s->tmp = NULL;
	}
}

static inline void	_init_state_ptrs(t_state *s)
{
	s->current_parser = NULL;
	s->current_lexer = NULL;
	s->current_cmd = NULL;
	s->input = NULL;
	s->destroy_command = NULL;
	s->destroy_lexer = NULL;
	s->destroy_parser = NULL;
	s->destroy_token = NULL;
	s->tmp_tok_lst = NULL;
	s->tmp = NULL;
	s->pwd = NULL;
	s->prompt = DFL_PROMPT;
	s->got_heredoc = false;
	s->tmp_flag = 0;
	s->path = NULL;
	s->magic = STATE_MAGIC;
}

/* Note: fns structs are destroyed by allocator */
t_state	*init_state(char **envp)
{
	t_state	*s;

	if (!envp)
		return (NULL);
	s = malloc(sizeof(struct s_global_state));
	if (!s)
		return (NULL);
	s->current_exit_code = EX_HAPPY;
	s->error_code = 0;
	s->envp = envp;
	_init_state_ptrs(s);
	mem_mgr_init(&s->mem_mgr);
	mem_add_mem(&s->mem_mgr.list, s, sizeof(struct s_global_state));
	s->lex_fns = init_lex_fns(s);
	s->cmd_fns = init_cmd_fns(s);
	s->parse_fns = init_parse_fns(s);
	s->sh_env_list = copy_envp(&s->mem_mgr, envp);
	if (!s->sh_env_list)
		exit_clean(&get_mem(s)->list, ENOMEM, __FUNCTION__, EMSG_MALLOC);
	init_env_vars(s);
	sig_set_handlers(INT_DFL);
	return (s);
}

/* Destroys parser before lexer
 * as parser wraps some lexer token content
 */
void	destroy_state(t_state **state)
{
	t_mem_mgr	*m;
	t_state		*s;

	s = *state;
	m = get_mem(s);
	if (s->current_parser)
		s->destroy_parser(m, (void **)&s->current_parser);
	if (s->current_lexer)
		s->destroy_lexer(m, (void **)&s->current_lexer);
	if (s->current_cmd)
		s->destroy_command(m, (void **)&s->current_cmd);
	if (s->input)
		m->dealloc(&m->list, s->input);
	if (s->sh_env_list)
		env_free_list(m, s->sh_env_list);
	if (s->pwd)
		m->dealloc(&m->list, s->pwd);
	mem_free_all(&s->mem_mgr.list);
	*state = NULL;
}
