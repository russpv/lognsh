#include "token_int.h"

// Passed to token llist iterator to collect word parts
// on static str with successive calls.
// Discards null raw strings.
// c must be a t_tok
int	tok_do_grp_combine(t_state *s, void *c)
{
	const t_tok	*content = (t_tok *)c;
	static char	*str = NULL;
	char		*tmp_str;
	t_list		*tok_lst;

	tmp_str = NULL;
	if (NULL == c)
	{
		debug_print(_MOD_ ": %s: got NULL\n", __FUNCTION__);
		free(str);
		str = NULL;
		return (0);
	}
	if (str)
		debug_print(_MOD_ ": %s: got _%s_, cache=%s\n", __FUNCTION__, content->t.tok.raw, str);
	else
		debug_print(_MOD_ ": %s: got _%s_\n", __FUNCTION__, content->t.tok.raw);
	if (NULL == content->t.tok.raw)
		return (0);
	if (0 != *(content->t.tok.raw))
	{
		if (true == content->t.tok.is_combinable)
		{
			debug_print(_MOD_ ": %s: got combinable _%s_\n", __FUNCTION__, content->t.tok.raw);
			if (str)
				tmp_str = ft_strjoin(str, content->t.tok.raw);
			else
				tmp_str = ft_strdup(content->t.tok.raw);
		}
		else
			tmp_str = ft_strdup(content->t.tok.raw);
		if (!tmp_str)
			exit_clean(&get_mem(s)->list, ENOMEM, __FUNCTION__, EMSG_MALLOC);
	}
	// commit if we have something and !is_split_bndary
	if (str && false == content->t.tok.is_combinable)
	{
		tok_lst = get_tmp_tok_list(s);
		ft_lstadd_back(&tok_lst, ft_lstnew_tmp(get_mem(s), create_tmp_token(get_mem(s), str)));
	}
	if (tmp_str) //save to str and s->tmp
	{
		if (str)
			free(str);
		str = tmp_str;
		debug_print(_MOD_ ": %s: returning _%s_\n", __FUNCTION__, str);
		set_tmp(s, str);
	}
	return (0);
}
