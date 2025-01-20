#include "lex_int.h"

/* add to llist tail a new token, clear buf */
int	add_token(t_lex *lexer, t_tok *token)
{
	debug_print("add_token\n");
	if (token && lexer)
	{
		memset(lexer->buf, 0, LEX_BUFSZ);
		ft_lstadd_back(&lexer->token_list, ft_lstnew(token));
		return (0);
	}
	return (1);
}

/* Creates token based on current buf and ptr, does not add to llist */
t_tok	*lex_create_token(t_lex *lexer, int type)
{
	t_tok	*token;

	debug_print("lex_create_token\n");
	if (!lexer)
		return (NULL);
	token = create_token(lexer->buf, type, (size_t)(lexer->ptr
				- lexer->raw_string));
	if (token)
	{
		if (DO_GLOBBING == lexer->do_globbing)
			tok_set_globbing(token);
		if (DO_EXPANSION == lexer->do_expansion)
			tok_set_expansion(token);
		lexer->do_globbing = RESET;
		lexer->do_expansion = RESET;
	}
	return (token);
}
