#include "parse_int.h"

t_parse_fns	*init_parse_fns(t_state *s)
{
	const t_mem_mgr	*m = get_mem(s);
	t_parse_fns		*fns;

	fns = m->f(&((t_mem_mgr *)m)->list, sizeof(t_parse_fns));
	if (!fns)
		exit_clean(&((t_mem_mgr *)m)->list, ENOMEM, __FUNCTION__, EMSG_MALLOC);
	fns->p_do_redirections = p_do_redirections;
	return (fns);
}

static inline void	_init_parser(t_state *s, t_parser *p)
{
	p->ast = NULL;
	p->curr_idx = -1;
	p->curr_cmd = NULL;
	p->last_node = NULL;
	p->ref_node = NULL;
	p->parse_error = false;
	p->global_state = s;
	p->mmgr = get_mem(s);
	register_parser_destroy(s, destroy_parser);
}

t_parser	*create_parser(t_state *s, t_list *tokens)
{
	t_parser	*p;

	if (!tokens)
		return (NULL);
	p = (t_parser *)get_mem(s)->f(&get_mem(s)->list, sizeof(t_parser));
	if (p)
	{
		p->tokens = ft_lstcopy_tmp(get_mem(s), tokens, copy_token,
				destroy_token);
		if (!p->tokens)
			exit_clean(&get_mem(s)->list, ENOMEM, __FUNCTION__, EMSG_MALLOC);
		p->curr_tok = tokens;
		p->token_count = ft_lstsize(tokens);
		dprint("p->tokc=%d\n", p->token_count);
		_init_parser(s, p);
		p->st = st_int_create(get_mem(s));
		if (!p->st)
			exit_clean(&get_mem(s)->list, ENOMEM, __FUNCTION__, EMSG_MALLOC);
	}
	return (p);
}

void	destroy_parser(t_mem_mgr *m, void **instance)
{
	t_parser	*p;

	p = (t_parser *)(*instance);
	if (!p)
		return ;
	dprint(_MOD_ ": %s...\n", __FUNCTION__);
	if (p->ast)
	{
		destroy_ast_node(m, (void **)&p->ast);
		p->ast = NULL;
	}
	if (p->tokens)
	{
		dprint(_MOD_ ": %s: destroying p->tokens...\n", __FUNCTION__);
		ft_lstclear_tmp(m, &p->tokens, destroy_token);
		p->tokens = NULL;
	}
	if (p->st)
	{
		st_int_destroy(m, p->st);
		p->st = NULL;
	}
	m->dealloc(&m->list, p);
	*instance = NULL;
}
