
#include "token_int.h"

int	tok_add_subtok(t_mem_mgr *mgr, t_tok *grp, t_tok *sub)
{
	if (!grp || !sub)
		return (ERR_ARGS);
	ft_lstadd_back(&grp->t.meta.tokens, ft_lstnew_tmp(mgr, sub));
	return (0);
}
