#include "parse_int.h"

/* Returns true parser index is equal or gtr than token count or
 * token pointer (unconsumed) is a TOK_EOF
 */
bool	is_at_end(t_parser *p)
{
	return (p->curr_idx >= p->token_count || tok_get_type(peek(p)) == TOK_EOF);
}

/* Returns true if TOK_WORD, TOK_NAME, TOK_BI, TOK_ENV_VAR
 * or any reserved word, or TOK_EMPTY (for appropriate exec err)
 */
bool	is_cmd_token(t_tok *tok)
{
	const enum e_tok_type	type = tok_get_type(tok);

	return (type == TOK_GROUP_WORD || type == TOK_WORD || type == TOK_NAME
		|| type == TOK_EXIT_STATUS || type == TOK_ENV_VAR || type == TOK_CASE
		|| type == TOK_DO || type == TOK_DONE || type == TOK_ELSE
		|| type == TOK_ESAC || type == TOK_FI || type == TOK_IN
		|| type == TOK_BI || type == TOK_THEN || type == TOK_WHILE
		|| type == TOK_UNTIL || type == TOK_EMPTY);
}

/* Returns true if TOK_ANDIF or TOK_ORIF */
bool	is_log_token(t_tok *tok)
{
	const enum e_tok_type	type = tok_get_type(tok);

	return (type == TOK_ANDIF || type == TOK_ORIF);
}

/* Returns true if TOK_PIPE */
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

/* Specials */

bool	is_group_token(t_tok *tok)
{
	const enum e_tok_type	type = tok_get_type(tok);

	return (type == TOK_GROUP_WORD);
}

/* True if token type needs to be expanded.
 */
bool	is_expansion(t_tok *tok)
{
	const enum e_tok_type	type = tok_get_type(tok);

	return (type == TOK_EXIT_STATUS || type == TOK_ENV_VAR
		|| true == tok_get_expansion(tok));
}

bool	is_group_op_token(t_tok *tok)
{
	const enum e_tok_type	type = tok_get_type(tok);

	return (type == TOK_PIPE || type == TOK_ANDIF || type == TOK_ORIF);
}

bool	is_globbing(t_tok *tok)
{
	return (true == tok_get_globbing(tok));
}

/* Redirs */

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
	if ((n->type == AST_NODE_CMD && n->data.cmd.redc > 0)
		|| (n->type == AST_NODE_PROC && n->data.proc.redc > 0))
		return (true);
	return (false);
}

/* Procs */

bool	is_open_paren(t_tok *tok)
{
	return (tok_get_type(tok) == TOK_OPEN_PAREN);
}

bool	is_close_paren(t_tok *tok)
{
	return (tok_get_type(tok) == TOK_CLOSE_PAREN);
}

/* Args */

/* Added builtins so that they can be considered
 * args. Ex: exec echo haha, or cat echo
 * Note: TOK_EMPTY is valid arg.
 */
bool	is_arg_token(t_tok *tok)
{
	const enum e_tok_type	type = tok_get_type(tok);

	return (type == TOK_WORD || type == TOK_NAME || type == TOK_ENV_VAR
		|| type == TOK_EXIT_STATUS || type == TOK_BI || type == TOK_GROUP_WORD
		|| type == TOK_EMPTY);
}

/* Returns true if tok's raw string is an OP_OPTION (-) */
bool	is_option(t_tok *tok)
{
	const char	*s = tok_get_raw(tok);

	if (s && *s)
		return ((unsigned char)OP_OPTION == s[0]);
	return (false);
}
