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

char	*get_pwd(t_state *s)
{
	if (!s)
		return (NULL);
	return (s->pwd);
}

/* Returns the last exit code. */
int	*get_status(t_state *s)
{
	if (!s)
		return (NULL);
	return (&s->current_exit_code);
}

t_list	**get_tmp_tok_list(t_state *s)
{
	if (!s)
		return (NULL);
	return (&s->tmp_tok_lst);
}

int	get_err(t_state *s)
{
	if (!s)
		return (0);
	return (s->error_code);
}

/* Mem */

// Returns memory manager struct
t_mem_mgr	*get_mem(t_state *s)
{
	if (!s)
		return (NULL);
	return (&s->mem_mgr);
}

int	get_tmp_flag(t_state *s)
{
	if (!s)
		return (INT_MAX);
	return (s->tmp_flag);
}

char	*get_tmp(t_state *s)
{
	if (!s)
		return (NULL);
	return (s->tmp);
}

/* Fns */


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

/* Env */


/* Returns and caches new env heap array if cached env is null */
char	**get_envp(t_state *s)
{
	extern char	**environ;

	if (!s)
		return (NULL);
	if (!s->sh_env_list)
		s->sh_env_list = copy_envp(get_mem(s), environ);
	return (env_to_array(get_mem(s), s->sh_env_list));
}

/* Passthrough getter to retrieve individual v in k:v */
char	*get_env_val(t_state *s, const char *key)
{
	if (!s)
		return (NULL);
	if (!s->sh_env_list)
		return (NULL);
	return (env_find_value(key, s->sh_env_list));
}

t_env	*get_env_list(t_state *s)
{
	if (!s)
		return (NULL);
	return (s->sh_env_list);
}

t_env	**get_env_list_ptr(t_state *s)
{
	if (!s)
		return (NULL);
	return (&s->sh_env_list);
}

char	**get_path(t_state *s)
{
	extern char	**environ;

	if (!s)
		return (NULL);
	if (!s->sh_env_list)
		s->sh_env_list = copy_envp(get_mem(s), environ);
	if (NULL == s->path)
		set_path(s, env_find_value(PATH_KEY, s->sh_env_list));
	print_array(s->path);
	return (s->path);
}

/* Returns true if state's sh_env_list is non-NULL */
bool	has_sh_envp(t_state *s)
{
	if (!s)
		return (false);
	return (NULL != s->sh_env_list);
}

