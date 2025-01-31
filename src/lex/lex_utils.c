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
	token = create_token(lexer->buf, type, (size_t)(lexer->ptr \
				- lexer->raw_string));
	if (token)
	{
		if (IN_DOUBLE_QUOTES == lexer->state)
			tok_set_dquotes(token);
		if (true == lexer->do_globbing)
			tok_set_globbing(token);
		if (true == lexer->do_expansion)
			tok_set_expansion(token);
		lexer->do_globbing = INIT;
		lexer->do_expansion = INIT;
	}
	return (token);
}
inline bool	is_too_long(const char *input)
{
	if (NULL == input)
		return (true);
	if (ft_strlen(input) >= LEX_BUFSZ)
	{
		debug_print("ERROR: Input exceeds buf size.\n");
		return (true);
	}
	return (false);
}

bool	lex_get_incomplete(t_lex *lexer)
{
	return (lexer->is_incomplete);
}

t_list	*lex_get_tokens(t_lex *lexer)
{
	return (lexer->token_list);
}
