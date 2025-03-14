#include "lex_int.h"

// Assumes ptr is on '$', and we keep the '$'
// Only delimited by end of valid varname.
// If not normally delimited, is a subtoken.
int	tokenize_dollar(t_state *s, t_lex *lexer)
{
	t_tok	*token;

	debug_print(_MOD_ YELLOW": STATE: %s, ptr:_%c_\n"RESET, __FUNCTION__, *lexer->ptr);
	// TODO handle '?' and other dollar specials
	put_on_buf(lexer);
	while (lexer->ptr && is_varnamechar((unsigned char)(*lexer->ptr)))
		put_on_buf(lexer);
	if (0 == ft_strlen(lexer->buf))
		return (0);
	lexer->do_expansion = true;
	if (false == is_normal_delim(lexer, 0))
		lexer->is_subtoken = true;
	token = lex_create_token(get_mem(s), lexer, TOK_ENV_VAR);
	if (NULL == token)
		return (ERR_GENERAL);
	if (0 != add_token(get_mem(s), lexer, token))
		return (ERR_GENERAL);
	return (0);
}
