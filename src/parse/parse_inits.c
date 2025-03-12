#include "parse_int.h"

static inline void	_init_parser(t_state *s, t_parser *p)
{
	p->ast = NULL;
	p->curr_idx = -1;
	p->curr_cmd = NULL;
	p->last_node = NULL;
	p->ref_node = NULL;
	p->parse_error = false;
	p->global_state = s;
	register_parser_destroy(s, destroy_parser);
}

t_parser	*create_parser(t_state *s, t_list *tokens)
{
	t_parser	*p;

	if (!tokens)
		return (NULL);
	p = (t_parser *)malloc(sizeof(t_parser));
	if (p)
	{
		p->tokens = ft_lstcopy_tmp(get_mem(s), tokens, copy_token, destroy_token);
		if (!p->tokens)
		{
			free(p);
			return (NULL);
		}
		p->curr_tok = tokens;
		p->token_count = ft_lstsize(tokens);
		debug_print("p->tokc=%d\n", p->token_count);
		_init_parser(s, p);
		p->st = st_int_create();
		if (!p->st)
		{
			free(p);
			return (NULL);
		}
	}
	return (p);
}

void	destroy_parser(t_state *s, void *instance)
{
	t_parser	*p;

	p = (t_parser *)instance;
	if (!p)
		return ;
	debug_print(_MOD_ ": %s...\n", __FUNCTION__);
	if (p->ast)
	{
		destroy_ast_node(get_mem(s), p->ast);
		p->ast = NULL;
	}
	if (p->tokens)
	{
		debug_print(_MOD_ ": %s: destroying p->tokens...\n", __FUNCTION__);
		ft_lstclear_tmp(get_mem(s), &p->tokens, destroy_token);
		p->tokens = NULL;
	}
	if (p->st)
	{
		st_int_destroy(p->st);
		p->st = NULL;
	}
	free(p);
}
