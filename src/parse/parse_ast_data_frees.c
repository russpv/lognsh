#include "parse_int.h"

/* Fully frees a struct s_redir, for use in llist destroy */
void	destroy_redir(void *in)
{
	t_redir_data	*redir;

	redir = (t_redir_data *)in;
	if (NULL == redir)
		return ;
	debug_print(_MOD_ ": %s...\n", __FUNCTION__);
	if (redir->symbol)
		free(redir->symbol);
	if (redir->filename)
		free(redir->filename);
	if (redir->heredoc_body)
		free(redir->heredoc_body);
	if (redir->lst_glob)
		ft_lstclear(&redir->lst_glob, free);
	free(redir);
}

/* Fully frees a struct s_arg, for use in llist destroy */
void	destroy_arg(t_mem_mgr *mgr, void *in)
{
	t_arg_data	*arg;

	if (!in || !mgr)
		return ;
	arg = (t_arg_data *)in;
	debug_print(_MOD_ ": %s...\n", __FUNCTION__);
	if (arg->lst_tokens)
		ft_lstclear_tmp(mgr, &arg->lst_tokens, destroy_token);
	if (arg->raw)
	{
		mgr->dealloc(&mgr->list, arg->raw);
		arg->raw = NULL;
	}
	if (arg->tmp)
		ft_freearr_mem(&mgr->list, mgr->dealloc, (void **)arg->tmp, -1);
	mgr->dealloc(&mgr->list, arg);
}
