#include "lex_int.h"

/* terminates a lexer run */
int	tokenize_null(t_state *s, t_lex *lexer)
{
	t_tok	*token;

	dprint(_MOD_ YELLOW ": STATE: %s, ptr:_%c_\n" RESET, __FUNCTION__,
		*lexer->ptr);
	if (lexer)
	{
		if (lexer->last_grp_tok)
			add_grptoken(get_mem(s), lexer);
		lexer->is_subtoken = false;
		token = create_token(get_mem(s), "\0", TOK_EOF, (size_t)(lexer->ptr
					- lexer->raw_string));
		if (token)
			add_token(get_mem(s), lexer, token);
		else
			return (ERR_GENERAL);
	}
	return (0);
}
