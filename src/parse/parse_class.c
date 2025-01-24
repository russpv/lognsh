#include "parse_int.h"

void *destroy_cmd_node(t_ast_node *n)
{
	//TODO
	(void)n;
	return (NULL);
}

void *destroy_args(t_arg_data *args, t_ast_node_cmd *cmd)
{
	(void)args;
	(void)cmd;
	//TODO
	return (NULL);
}

t_parser *create_parser(t_list *tokens)
{
	if (!tokens)
		return (NULL);
	t_parser *p = (t_parser *)malloc(sizeof(t_parser));
	if (p)
	{
		p->tokens = tokens;
		p->curr_idx = -1;
		p->curr_tok = tokens;
		p->token_count = ft_lstsize(tokens);
		p->curr_cmd = NULL;
		p->last_node = NULL;
		p->ref_node = NULL;
		p->parse_error = false;
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
	//TODO
	(void)p;
}