#include "parse_int.h"

static t_redir_data	*_init_redir(t_ast_node *target, enum e_tok_type type)
{
	t_redir_data	*red;

	if (!target)
		return (NULL);
	red = malloc(sizeof(t_redir_data));
	if (!red)
	{
		err("Memory allocation failed for redirection data\n");
		return (NULL);
	}
	red->type = type;
	red->target_ptr = target;
	red->do_globbing = false;
	red->do_expansion = false;
	red->heredoc_body = NULL;
	red->symbol = NULL;
	red->filename = NULL;
	red->global_state = NULL;
	return (red);
}

// Stores redirection symbol, globbing, expansion, filename, and advances parser
/* tok: current redirect token returned from advance()
 * Freeing handled by caller in case of error
 */
static int	_process_normal_redir(t_parser *p, t_tok *tok, t_redir_data *red)
{
	t_tok	*tok_name;

	if (!p || !red || !tok)
		return (ERR_ARGS);
	red->symbol = ft_strdup(tok_get_raw((t_tok *)tok));
	if (!red->symbol)
		return (err("Allocation for redirection symbol failed\n"), ERR_MEM);
	debug_print("Parser: Redirection: type=%d symbol=%s\n", red->type,
		red->symbol);
	if (is_at_end(p))
		return (err("Expected a filename after redirection operator,\
				found EOF\n"), ERR_SYNTAX);
	tok_name = advance(p);
	debug_print("Parser: _process_normal_redir filename:%s\n",
		tok_get_raw(tok_name));
	if (!(is_filename_token((t_tok *)tok_name)
			|| is_expansion((t_tok *)tok_name)))
		return (err("Expected a valid filename after redirection operator\n"),
			ERR_SYNTAX);
	red->do_globbing = tok_get_globbing((t_tok *)tok_name);
	red->do_expansion = tok_get_expansion((t_tok *)tok_name);
	red->filename = ft_strdup(tok_get_raw((t_tok *)tok_name));
	if (!red->filename)
		return (err("Allocation for redirection filename failed\n"), ERR_MEM);
	return (0);
}

/* Copies TOK_HEREDOC_WORD content to redir data
 * Returns ERR_MEM if malloc fails
 * Freeing handled by caller
 */
static int	_process_heredoc_redir(t_redir_data *red, t_tok *tok)
{
	if (!red || !tok)
		return (ERR_ARGS);
	debug_print("Parser: Got heredoc document\n");
	red->symbol = NULL;
	red->filename = NULL;
	red->heredoc_body = ft_strdup(tok_get_raw((t_tok *)tok));
	if (!red->heredoc_body)
	{
		err("Memory allocation failed for redirection heredoc\n");
		return (ERR_MEM);
	}
	return (0);
}

/* Freeing handled by caller */
static int	_add_redir(t_ast_node *node, t_redir_data *red)
{
	t_list	*new;

	if (!red || !node)
		return (ERR_ARGS);
	new = ft_lstnew(red);
	if (!new)
	{
		err("Failed to create execution node for redirection\n");
		return (ERR_MEM);
	}
	debug_print("Parser: Adding redirection: (%s %s | doc:%s glob:%d exp:%d)\n",
		red->symbol, red->filename, red->heredoc_body, red->do_globbing,
		red->do_expansion);
	ft_lstadd_back(p_get_redirs_ptr(node), new);
	p_update_redc(node, 1);
	return (0);
}

/* Consumes redir tokens and adds them to node (arg 2) linked list
 * Returns NULL if syntax error
 * Heredocs already turned into TOK_HEREDOC_WORD
 */
static int	_parse_redir(t_parser *p, t_ast_node *node)
{
	t_redir_data	*red;
	t_tok			*tok;

	if (!p || !node)
		return (ERR_ARGS);
	while (!is_at_end(p) && is_redir_token(peek(p)))
	{
		tok = advance(p);
		red = _init_redir(node, tok_get_type((t_tok *)tok));
		if (false == is_heredoc_token((t_tok *)tok))
		{
			if (0 != _process_normal_redir(p, tok, red))
				return (destroy_redir(red), ERR_GENERAL);
		}
		else
		{
			if (0 != _process_heredoc_redir(red, tok))
				return (destroy_redir(red), ERR_GENERAL);
		}
		if (0 != _add_redir(node, red))
			return (destroy_redir(red), ERR_GENERAL);
	}
	debug_print("Parser: _parse_redir: curr peek tok: %s\n",
		tok_get_raw(peek(p)));
	return (0);
}

/* PROCESS REDIR
 * Adds redirections to node's t_list
 * Upon failure, frees any downstream heap mem
 * and returns. Does nothing to args.
 */
int	process_redir(t_parser *p, t_ast_node *ast_node)
{
	if (is_redir_token(peek(p)))
	{
		if (0 != _parse_redir(p, ast_node))
		{
			err("Failed to parse redirection\n");
			return (ERR_GENERAL);
		}
	}
	else
		debug_print("Parser: Not a redir:%s\n", tok_get_raw(peek(p)));
	return (0);
}
