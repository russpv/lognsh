#include "parse_int.h"

/* Returns true parser index is equal or gtr than token count or
 * token pointer (unconsumed) is a TOK_EOF
 */
bool	is_at_end(t_parser *p)
{
	return (p->curr_idx >= p->token_count || tok_get_type(peek(p)) == TOK_EOF);
}

/* Returns true if TOK_WORD, TOK_NAME, TOK_BI, TOK_ENV_VAR
 * or any reserved word
 */
bool	is_cmd_token(t_tok *tok)
{
	const enum e_tok_type	type = tok_get_type(tok);

	return (type == TOK_WORD || type == TOK_NAME || type == TOK_BI
		|| type == TOK_EXIT_STATUS || type == TOK_ENV_VAR || type == TOK_CASE
		|| type == TOK_DO || type == TOK_DONE || type == TOK_ELSE
		|| type == TOK_ESAC || type == TOK_FI || type == TOK_IN
		|| type == TOK_THEN || type == TOK_WHILE || type == TOK_UNTIL);
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
