#include "parse_int.h"

void	*destroy_cmd_node(t_ast_node *n)
{
	// TODO
	(void)n;
	return (NULL);
}

void	*destroy_args(t_arg_data *args, t_ast_node_cmd *cmd)
{
	(void)args;
	(void)cmd;
	// TODO
	return (NULL);
}

void	destroy_ast(t_ast_node *ast)
{
	//TODO
	(void)ast;
}

void static inline _init_parser(t_parser *p)
{
	p->ast = NULL;
	p->curr_idx = -1;
	p->curr_cmd = NULL;
	p->last_node = NULL;
	p->ref_node = NULL;
	p->parse_error = false;
}

t_parser	*create_parser(t_list *tokens)
{
	t_parser	*p;

	if (!tokens)
		return (NULL);
	p = (t_parser *)malloc(sizeof(t_parser));
	if (p)
	{
		p->tokens = ft_lstcopy(tokens, copy_token_data, destroy_token);
		if (!p->tokens)
		{
			free(p);
			return (NULL);
		}
		p->curr_tok = tokens;
		p->token_count = ft_lstsize(tokens);
		_init_parser(p);
		p->st = st_create();
		if (!p->st)
		{
			free(p);
			return (NULL);
		}
	}
	return (p);
}

void	destroy_parser(t_parser *p)
{
	if (p->ast)
		destroy_ast(p->ast);
	if (p->tokens)
		ft_lstclear(p->tokens, destroy_token);
	if (p->st)
		st_destroy(p->st);
	free(p);
}
