#include "parse_int.h"

#define DBGMSG_GOTARGS _MOD_ ": p_do_globbing_toks - tok: %s, lst:%p\n"
#define DBGMSG_MATCHES _MOD_ ": p_do_globbing_toks found %d matches, 1st: %s\n"

// Tests whether globstar is preceded with escape char
static bool	_has_globbing(const char *raw)
{
	char *globstar;
	
	globstar =ft_strchr(raw, '*');
	if (NULL == globstar)
		return (false);
	if (globstar != raw)
		if ('\\' == *(globstar - 1))
			return (false);
	return (true);
}

int	p_do_globbing_toks(t_mem_mgr *mgr, t_list **lst_node, void *lst_c)
{
	t_tok		*tok;
	t_list		*lst;
	int			res;

	res = 0;
	lst = NULL;
	if (!lst_c)
		return (ERR_ARGS);
	tok = (t_tok *)lst_c;
	if (!tok_get_raw(tok))
		return (ERR_ARGS);
	debug_print(DBGMSG_GOTARGS, tok_get_raw(tok), (void *)lst_node);
	if (true == _has_globbing(tok_get_raw(tok)))
	{
		lst = match_glob(mgr, tok_get_raw(tok));
		if (lst)
		{
			debug_print(DBGMSG_MATCHES, ft_lstsize(lst), lst->content);
			res = do_tok_inserts(mgr, lst_node, &lst, create_tmp_token);
			if (0 != res)
				return (res);
		}
	}
	return (0);
}
