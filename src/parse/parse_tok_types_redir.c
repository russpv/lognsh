#include "parse_int.h"

/* Returns true if tok is an in, out, append or heredoc_word */
bool	is_redir_token(t_tok *tok)
{
	const enum e_tok_type	type = tok_get_type(tok);

	return (type == TOK_REDIRECT_APPEND || type == TOK_REDIRECT_IN
		|| type == TOK_REDIRECT_OUT || type == TOK_HEREDOC_WORD);
}

/* Returns true if tok is a heredoc_word */
bool	is_heredoc_token(t_tok *tok)
{
	if (!tok)
		return (false);
	return (tok_get_type(tok) == TOK_HEREDOC_WORD);
}

/* Returns true if tok is a name, word or heredoc_word */
bool	is_filename_token(t_tok *tok)
{
	const enum e_tok_type	type = tok_get_type(tok);

	return (type == TOK_GROUP_WORD || type == TOK_NAME || type == TOK_WORD
		|| type == TOK_HEREDOC_WORD);
}

/* Returns tree if is a command node with redc > 0 */
bool	node_has_redirects(t_ast_node *n)
{
	if ((n->type == AST_NODE_CMD && n->data.cmd.redc > 0) ||
		(n->type == AST_NODE_PROC && n->data.proc.redc > 0))
		return (true);
	return (false);
}
