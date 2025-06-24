#include "command_int.h"

void	destroy_cmd_fns(t_mem_mgr *m, t_cmd_fns *f)
{
	if (f && m)
		m->dealloc(&m->list, f);
}

t_cmd_fns	*init_cmd_fns(t_state *s)
{
	const t_mem_mgr	*m = get_mem(s);
	t_cmd_fns		*fns;

	fns = m->f(&((t_mem_mgr *)m)->list, sizeof(t_cmd_fns));
	if (!fns)
		exit_clean(&((t_mem_mgr *)m)->list, ENOMEM, __FUNCTION__, EMSG_MALLOC);
	fns->c_get_argv = c_get_argv;
	fns->c_get_argvc = c_get_argvc;
	fns->c_get_cmdc = c_get_cmdc;
	fns->c_get_node = c_get_node;
	fns->c_get_fullpath = c_get_fullpath;
	fns->c_get_ctxtst = c_get_ctxtst;
	fns->c_get_fildes = c_get_fildes;
	fns->save_redirs = save_redirs;
	fns->restore_redirs = restore_redirs;
	return (fns);
}

void	destroy_cmd(t_mem_mgr *m, void **c)
{
	t_cmd	*cmd;

	cmd = (t_cmd *)(*c);
	if (cmd->st)
		st_int_destroy(m, cmd->st);
	if (cmd->fildes)
		ft_freearr_mem(&m->list, m->dealloc, (void **)cmd->fildes, -1);
	if (cmd->argv)
		ft_freearr_mem(&m->list, m->dealloc, (void **)cmd->argv, -1);
	m->dealloc(&m->list, cmd);
	*c = NULL;
}

t_cmd	*init_cmd(t_state *s, t_ast_node *a)
{
	t_cmd	*c;

	c = get_mem(s)->f(&get_mem(s)->list, sizeof(struct s_cmd));
	if (c)
	{
		c->fildes = NULL;
		c->st = st_int_create(get_mem(s));
		c->argv = NULL;
		c->argc = 0;
		c->redirs = NULL;
		c->redc = 0;
		c->fullpath = NULL;
		c->curr_node = a;
		c->saved_stderr = -1;
		c->saved_stdin = -1;
		c->saved_stdout = -1;
		c->curr_cmdc = -1;
		c->argvc = 0;
		s_free_tmp(s);
		set_command(s, c);
		register_command_destroy(s, destroy_cmd);
	}
	return (c);
}
