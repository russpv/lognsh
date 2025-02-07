#include "parse_int.h"

/* Deletes llist content */
void	destroy_redir(void *in)
{
	t_redir_data *redir = (t_redir_data *)in;
	debug_print("destroy_redir...\n");

	if (redir->symbol)
		free(redir->symbol);
//	if (redir->filename)
//		free(redir->filename); //note: free lexer to clean
	if (redir->doc)
		free(redir->doc);
}

/* Deletes llist content */
void	destroy_arg(void *in)
{
	t_arg_data *arg = (t_arg_data *)in;
	debug_print("destroy_arg...\n");

	if (arg->raw)
		free(arg->raw);
	if (arg->tmp)
		ft_freearr((void **)arg->tmp, -1);
}

void	destroy_cmd_node(void *n)
{
	t_ast_node *node = (t_ast_node *)n;

	debug_print("destroy_cmd_node...\n");

	if (node->type != AST_NODE_CMD)
		return ;
	if (node->data.cmd.name)
		free(node->data.cmd.name);
	if (node->data.cmd.args)
		ft_lstclear(&node->data.cmd.args, destroy_arg);
	if (node->data.cmd.redirs)
		ft_lstclear(&node->data.cmd.redirs, destroy_redir);
	free(node);
}

void	destroy_proc_node(void *n)
{
	t_ast_node *node = (t_ast_node *)n;

	debug_print("destroy_proc_node...\n");

	if (AST_NODE_PROC != node->type)
		return ;
	ft_lstclear(&node->data.proc.cmds, destroy_ast);
}

// destroy t_list cmds, char **operators
void	destroy_log_node(void *n)
{
	t_ast_node *node = (t_ast_node *)n;

	debug_print("destroy_log_node...\n");

	if (AST_NODE_LOG != node->type)
		return ;
	if (node->data.log.operators)
		ft_freearr((void **)node->data.log.operators, -1);
	ft_lstclear(&node->data.log.cmds, destroy_ast);
}

void	destroy_pipe_node(void *n)
{
	t_ast_node *node = (t_ast_node *)n;

	debug_print("destroy_pipe_node...\n");

	if (AST_NODE_PIPELINE != node->type)
		return ;
	ft_lstclear(&node->data.pipe.cmds, destroy_ast);
}

void	destroy_ast(void *node)
{
	t_ast_node *ast = (t_ast_node *)node;

	debug_print("destroy_ast...\n");

	if (AST_NODE_PROC == ast->type)
		destroy_proc_node(ast);
	else if (AST_NODE_CMD == ast->type)
		destroy_cmd_node(ast);
	else if (AST_NODE_PIPELINE == ast->type)
		destroy_pipe_node(ast);
	else
		destroy_log_node(ast);
	
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
		p->tokens = ft_lstcopy(tokens, copy_token, destroy_token);
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
	if (!p)
		return ;
	debug_print("destroy_parser...\n");
	if (p->ast)
	{
		destroy_ast(p->ast);
		p->ast = NULL;
	}
	if (p->tokens)
	{
		debug_print("destroy_parser destroying p->tokens...\n");
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
