#include "parse_int.h"

/* Returns true parser index is equal or gtr than token count or
 * token pointer (unconsumed) is a TOK_EOF
 */
bool	is_at_end(t_parser *p)
{
	debug_print("\tParser: is_at_end found TOK_EOF\n");
	return (p->curr_idx >= p->token_count || tok_get_type(peek(p)) == TOK_EOF);
}

/* Returns true if tok's raw string is an OP_OPTION (-) */
bool	is_option(t_tok *tok)
{
	const char	*s = tok_get_raw(tok);

	if (s && *s)
		return ((unsigned char)OP_OPTION == s[0]);
	return (false);
}

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

	return (type == TOK_NAME || type == TOK_WORD || type == TOK_HEREDOC_WORD);
}
