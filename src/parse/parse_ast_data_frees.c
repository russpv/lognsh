#include "parse_int.h"

/* Fully frees a struct s_redir, for use in llist destroy */
void	destroy_redir(void *in)
{
	t_redir_data	*redir;

	redir = (t_redir_data *)in;
	if (NULL == redir)
		return ;
	debug_print("Parser: destroy_redir...\n");
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
void	destroy_arg(void *in)
{
	t_arg_data	*arg;

	arg = (t_arg_data *)in;
	debug_print("Parser: destroy_arg...\n");
	if (NULL == arg)
		return ;
	if (arg->raw)
		free(arg->raw);
	if (arg->tmp)
		ft_freearr((void **)arg->tmp, -1);
	free(arg);
}
