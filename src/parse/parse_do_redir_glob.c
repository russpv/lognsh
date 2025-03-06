#include "parse_int.h"

#define EMSG_AMBIG "ERR multiple redirect targets globbed\n"
#define DBGMSG_REDIR_NODE "Parser: p_do_globbing_redirs got redir type: %d fn:%s doc:\
		%s do_glob:%d\n"
#define DBGMSG_REDIR_GLOB "Parser: p_do_globbing_redirs found:%s...\n"

/* The redir version of p_do_globbing_args. 
 * Operates on a single t_redir_data node. 
 */
int	p_do_globbing_redirs(void *c)
{
	t_redir_data		*r;
	char				*new_fn;

	r = (t_redir_data *)c;
	debug_print(DBGMSG_REDIR_NODE, r->type, r->filename,\
		r->heredoc_body, r->do_globbing);
	if (true == r->do_globbing)
	{
		r->lst_glob = match_glob((const char *)r->filename);
		if (r->lst_glob)
		{
			debug_print(DBGMSG_REDIR_GLOB,(char *)r->lst_glob->content);
			if (ft_lstsize(r->lst_glob) > 1)
				return (err(EMSG_AMBIG), ERR_AMBIGUOUS_REDIR);
			else
			{
				new_fn = ft_strdup(r->lst_glob->content);
				if (!new_fn)
					return (ERR_MEM);
				free(r->filename);
				r->filename = new_fn;
			}
		}
	}
	return (0);
}
