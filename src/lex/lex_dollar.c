#include "lex_int.h"

// Assumes ptr is on '$', and we keep the '$'
// Only delimited by end of valid varname.
int	tokenize_dollar(t_lex *lexer)
{
	t_tok	*token;

	debug_print(_MOD_": STATE: %s, ptr:_%c_\n", __FUNCTION__, *lexer->ptr);
	// TODO handle '?' and other dollar specials
	put_on_buf(lexer);
	while (lexer->ptr && is_varnamechar((unsigned char)(*lexer->ptr)))
		put_on_buf(lexer);
	lexer->do_expansion = true;
	token = lex_create_token(lexer, TOK_ENV_VAR);
	if (token)
		if (0 != add_token(lexer, token))
			return (ERR_GENERAL);
	return (0);
}
