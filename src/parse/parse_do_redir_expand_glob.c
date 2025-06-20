
#include "parse_int.h"

#define EM_AMBIG "ERR multiple redirect targets globbed\n"
#define DM_RDR_G "%s: %s got redir type: %d fn:%s doc:%s do_glob:%d\n"
#define DM_RDR_G2 "%s: p_do_globbing_redirs found:%s...\n"

/* The redir version of p_do_globbing_args.
 * Operates on a single t_redir_data node.
 */
int	p_do_globbing_redirs(t_mem_mgr *m, void *c)
{
	t_redir_data	*r;
	char			*new_fn;

	r = (t_redir_data *)c;
	dprint(DM_RDR_G, _MOD_, r->type, r->filename, r->heredoc_body,
		r->do_globbing);
	if (true == r->do_globbing)
	{
		r->lst_glob = match_glob(m, (const char *)r->filename);
		if (r->lst_glob)
		{
			dprint(DM_RDR_G2, _MOD_, (char *)r->lst_glob->content);
			if (ft_lstsize(r->lst_glob) > 1)
				return (err(EM_AMBIG), ERR_AMBIGUOUS_REDIR);
			else
			{
				new_fn = ft_strdup_mem(m, r->lst_glob->content);
				if (!new_fn)
					exit_clean(&m->list, ENOMEM, __FUNCTION__, EMSG_MALLOC);
				m->dealloc(&m->list, r->filename);
				r->filename = new_fn;
			}
		}
	}
	return (0);
}
