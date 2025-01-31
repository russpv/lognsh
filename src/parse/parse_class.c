#include "parse_int.h"


static void	_destroy_redirs(void *in)
{
	t_redir_data *redirs = (t_redir_data *)in;
	(void)redirs;
}
static void	_destroy_args(void *in)
{
	t_arg_data *args = (t_arg_data *)in;
	// TODO
	(void)args;
}

void	*destroy_cmd_node(t_ast_node *n)
{
	if (n->type != AST_NODE_CMD)
		return (NULL);
	if (n->data.cmd.name)
		free(n->data.cmd.name);
	if (n->data.cmd.args)
		ft_lstclear(&n->data.cmd.args, _destroy_args);
	if (n->data.cmd.redirs)
		ft_lstclear(&n->data.cmd.redirs, _destroy_redirs);
	return (NULL);
}


void	destroy_ast(t_ast_node *ast)
{
	//TODO
	(void)ast;
}

void static inline _init_parser(t_state *s, t_parser *p)
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
		p->tokens = ft_lstcopy(tokens, copy_token_data, destroy_token);
		if (!p->tokens)
		{
			free(p);
			return (NULL);
		}
		p->curr_tok = tokens;
		p->token_count = ft_lstsize(tokens);
		_init_parser(s, p);
		p->st = st_create();
		if (!p->st)
		{
			free(p);
			return (NULL);
		}
	}
	return (p);
}

void	destroy_parser(void *instance)
{
	t_parser *p = (t_parser *)instance;
	if (p->ast)
	{
		destroy_ast(p->ast);
		p->ast = NULL;
	}
	if (p->tokens)
	{
		ft_lstclear(&p->tokens, destroy_token);
		p->tokens = NULL;
	}
	if (p->st)
	{
		st_destroy(p->st);
		p->st = NULL;
	}
	free(p);
}
