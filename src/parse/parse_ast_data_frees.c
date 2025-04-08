#include "parse_int.h"

/* Fully frees a struct s_redir, for use in llist destroy */
void	destroy_redir(t_mem_mgr *m, void **in)
{
	t_redir_data	*redir;

	redir = (t_redir_data *)(*in);
	if (NULL == redir)
		return ;
	dprint(_MOD_ ": %s...\n", __FUNCTION__);
	if (redir->symbol)
		m->dealloc(&m->list, redir->symbol);
	if (redir->filename)
		m->dealloc(&m->list, redir->filename);
	if (redir->heredoc_body)
		m->dealloc(&m->list, redir->heredoc_body);
	if (redir->lst_glob)
		ft_lstclear_str_tmp(m, &redir->lst_glob);
	if (redir->lst_tokens)
		ft_lstclear_tmp(m, &redir->lst_tokens, destroy_token);
	m->dealloc(&m->list, redir);
	*in = NULL;
}

/* Fully frees a struct s_arg, for use in llist destroy */
void	destroy_arg(t_mem_mgr *mgr, void **in)
{
	t_arg_data	*arg;

	if (!in || !mgr)
		return ;
	arg = (t_arg_data *)(*in);
	dprint(_MOD_ ":   %s...\n", __FUNCTION__);
	if (arg->lst_tokens)
		ft_lstclear_tmp(mgr, &arg->lst_tokens, destroy_token);
	if (arg->raw)
		mgr->dealloc(&mgr->list, arg->raw);
	if (arg->tmp)
		ft_freearr_mem(&mgr->list, mgr->dealloc, (void **)arg->tmp, -1);
	mgr->dealloc(&mgr->list, arg);
	*in = NULL;
}
