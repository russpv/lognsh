#include "parse_int.h"

/* Moves ptr and returns previous object */
t_tok	*advance(t_parser *p)
{
	t_tok *tmp = p->curr_tok->content;
	if (false == is_at_end(p))
	{
		p->curr_idx++;
		p->curr_tok = p->curr_tok->next;
	}
	return (tmp);
}

t_tok	*peek(t_parser *p)
{
	return (p->curr_tok->content);
}

t_tok	*lookahead(t_parser *p)
{
	if (is_at_end(p))
		return (NULL);
	else
		return (p->curr_tok->next->content);
}

t_tok	*previous(t_parser *p)
{
	return (p->curr_tok->prev->content);
}

bool	is_at_end(t_parser *p)
{
	return (p->curr_idx >= p->token_count || tok_get_type(peek(p)) == TOK_EOF);
}

bool	is_option(t_tok *tok)
{
	const char	*s = tok_get_raw(tok);

	if (s && *s)
		return ((unsigned char)OP_OPTION == s[0]);
	return (false);
}

bool	is_redir_token(t_tok *tok)
{
	const enum e_tok_type	type = tok_get_type(tok);

	return (type == TOK_REDIRECT_APPEND || type == TOK_REDIRECT_IN
		|| type == TOK_REDIRECT_OUT || type == TOK_HEREDOC_WORD);
}

bool	is_heredoc_token(t_tok *tok)
{
	if (!tok)
		return (false);
	return (tok_get_type(tok) == TOK_HEREDOC_WORD);
}

bool	is_filename_token(t_tok *tok)
{
	const enum e_tok_type	type = tok_get_type(tok);

	return (type == TOK_NAME || type == TOK_WORD || type == TOK_HEREDOC_WORD);
}

bool	is_open_paren(t_tok *tok)
{
	return (tok_get_type(tok) == TOK_OPEN_PAREN);
}

bool	is_cmd_token(t_tok *tok)
{
	const enum e_tok_type	type = tok_get_type(tok);

	return (type == TOK_WORD || type == TOK_NAME || type == TOK_BI
		|| type == TOK_EXIT_STATUS);
}
bool	is_log_token(t_tok *tok)
{
	const enum e_tok_type	type = tok_get_type(tok);

	return (type == TOK_ANDIF || type == TOK_ORIF);
}

bool	is_pipe_token(t_tok *tok)
{
	return (tok_get_type(tok) == TOK_PIPE);
}

bool	is_op_token(t_tok *tok)
{
	const enum e_tok_type	type = tok_get_type(tok);

	return (type == TOK_REDIRECT_IN || type == TOK_REDIRECT_OUT
		|| type == TOK_REDIRECT_APPEND || type == TOK_PIPE || type == TOK_ANDIF
		|| type == TOK_ORIF);
}

bool	is_arg_token(t_tok *tok)
{
	const enum e_tok_type	type = tok_get_type(tok);

	return (type == TOK_WORD || type == TOK_NAME);
}

void	parse_print(t_ast_node *ast)
{
	(void)ast;
	// TODO call printer methods
	// ast_traverse
}

bool	last_parsed_was_group_or_null(t_parser *p)
{
	if (NULL == p->last_node)
		return (true);
	return (p->last_node->type == AST_NODE_PIPELINE
		|| p->last_node->type == AST_NODE_PROC
		|| p->last_node->type == AST_NODE_LOG);
}

bool	last_parsed_was_pipe_or_cmd(t_parser *p)
{
	if (!p->last_node)
		return (false);
	return (p->last_node->type == AST_NODE_PIPELINE
		|| p->last_node->type == AST_NODE_CMD);
}

bool	last_parsed_was_cmd_or_proc(t_parser *p)
{
	if (!p->last_node)
		return (false);
	return (p->last_node->type == AST_NODE_CMD
		|| p->last_node->type == AST_NODE_PROC);
}

bool	last_parsed_was_cmd(t_parser *p)
{
	if (!p->last_node)
		return (false);
	return (p->last_node->type == AST_NODE_CMD);
}