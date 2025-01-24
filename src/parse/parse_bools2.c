#include "parse_int.h"


bool	is_open_paren(t_tok *tok)
{
	return (tok_get_type(tok) == TOK_OPEN_PAREN);
}

bool	is_cmd_token(t_tok *tok)
{
	const enum e_tok_type	type = tok_get_type(tok);

	return (type == TOK_WORD || type == TOK_NAME || type == TOK_BI
		|| type == TOK_EXIT_STATUS || type == TOK_ENV_VAR \
		|| type == TOK_CASE || type == TOK_DO \
		|| type == TOK_DONE || type == TOK_ELSE || type == TOK_ESAC \
		|| type == TOK_FI || type == TOK_IN || type == TOK_THEN \
		|| type == TOK_WHILE || type == TOK_UNTIL);
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
