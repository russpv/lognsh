#include "state_int.h"

void	set_parser(t_state *state, t_parser *p)
{
	state->current_parser = p;
}

void	set_lexer(t_state *state, t_lex *l)
{
	state->current_lexer = l;
}

void	set_command(t_state *s, t_cmd *c)
{
	s->current_cmd = c;
}

void	set_input(t_state *s, char *input)
{
	int	len;

	len = 0;
	if (!s)
		return ;
	if (NULL != input)
		len = ft_strnlen(input, MAX_INPUT_SZ);
	if (len == MAX_INPUT_SZ)
		return ;
	s->input = input;
	if (s == NULL || s->magic != STATE_MAGIC)
	{
		dprint("Bad state in set_input! s=%p, magic=0x%X\n", s,
			s->magic);
		exit_clean(&s->mem_mgr.list, ENOMEM, __FUNCTION__, NULL);
	}
	if (NULL != input)
		mem_add_mem(&s->mem_mgr.list, input, len + 1);
}

void	set_got_heredoc(t_state *s)
{
	if (NULL == s)
		return ;
	s->got_heredoc = true;
}

/* Cache */

void	set_tmp(t_state *s, char *str)
{
	if (NULL != s->tmp)
		(get_mem(s))->dealloc(&get_mem(s)->list, s->tmp);
	s->tmp = ft_strdup_mem(get_mem(s), str);
}

void	set_tmp_flag(t_state *s, int val)
{
	if (NULL == s)
		return ;
	s->tmp_flag = val;
}

/* Env */

int	set_oldpwd(t_state *s, const char *caller)
{
	char	*old_pwd;

	old_pwd = get_pwd(s);
	if (!old_pwd || 0 != env_upsert_value(get_mem(s), get_env_list(s),
			OLDPWD_KEY, old_pwd))
	{
		print_custom_err(caller, "Corrupted working directory.\n");
		return (ERR_GENERAL);
	}
	return (0);
}

void	set_pwd(t_state *s)
{
	t_env	*lst;
	char	*pwd;
	char	*tmp;
	char	*tmp2;

	if (!s)
		return ;
	lst = get_env_list(s);
	if (!lst)
		return ;
	pwd = getcwd(NULL, 0);
	if (!pwd)
		return (perror("Cannot retrieve working directory\n"));
	tmp = ft_strdup_mem(get_mem(s), pwd);
	if (!tmp)
		exit_clean(&get_mem(s)->list, ENOMEM, __FUNCTION__, EMSG_MALLOC);
	free(pwd);
	env_upsert_value(get_mem(s), lst, CWD_KEY, tmp);
	tmp2 = ft_strdup_mem(get_mem(s), tmp);
	s->pwd = tmp2;
	dprint("Set pwd:%s\n", s->pwd);
}

// Updates internal PATH with string delimited by :
void	set_path(t_state *s, const char *value)
{
	struct s_mem_utils	mem;

	if (!s)
		return ;
	mem_struct_init(get_mem(s), &mem);
	dprint(_MOD_ ": setting path\n");
	if (NULL != s->path)
		ft_freearr_mem(mem.head, mem.u, (void **)s->path, -1);
	dprint(_MOD_ ": got path:%s\n", value);
	if (value)
		s->path = ft_split_mem(&mem, value, ':');
	else
		s->path = NULL;
}

/* Exits */

// Sets error code
void	set_error(t_state *state, int code)
{
	state->error_code = code;
}

// Sets exit code
void	set_exit_status(t_state *state, int value)
{
	if (!state)
		return ;
	state->current_exit_code = value;
}

/* Vars */


static void	_set_prompt(t_state *s, char *str)
{
	t_env	*lst;

	if (!s || !str)
		return ;
	lst = get_env_list(s);
	if (lst)
		env_upsert_value(get_mem(s), lst, PROMPT_KEY, str);
}

static void	_set_lvl(t_state *s)
{
	t_env	*lst;
	char	*value;
	int		lvl;

	if (!s)
		return ;
	lst = get_env_list(s);
	if (!lst)
		return ;
	value = env_find_value(LVL_KEY, lst);
	if (!value)
		lvl = 1;
	else
		lvl = ft_atoi(value) + 1;
	value = ft_itoa_mem(&s->mem_mgr.list, s->mem_mgr.f, lvl);
	if (!value)
		exit_clean(&s->mem_mgr.list, ENOMEM, __FUNCTION__, EMSG_MALLOC);
	env_upsert_value(get_mem(s), lst, LVL_KEY, value);
}

void	init_env_vars(t_state *s)
{
	if (!s)
		return ;
	_set_prompt(s, DFL_PROMPT);
	_set_lvl(s);
	set_pwd(s);
}
