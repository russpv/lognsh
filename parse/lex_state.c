#include "lex_int.h"

static inline int	_handle_eof_state(t_lex *lexer)
{
	if (ON_EOF == lexer->state)
	{
		lexer->state = DONE;
		return (1);
	}
	return (0);
}

static inline int	_handle_quote_state(t_lex *lexer)
{
	if ((unsigned char)*lexer->ptr == OP_DQUOTE)
	{
		lexer->state = IN_DOUBLE_QUOTES;
		lexer->tokenizer = tokenize_double_quotes;
		return (1);
	}
	if ((unsigned char)*lexer->ptr == OP_SQUOTE)
	{
		lexer->state = IN_SINGLE_QUOTES;
		lexer->tokenizer = tokenize_single_quotes;
		return (1);
	}
	return (0);
}

static inline int	_handle_null_state(t_lex *lexer)
{
	if ((unsigned char)*lexer->ptr == OP_NULL)
	{
		debug_print("FOUND NULL TOKEN\n");
		lexer->state = ON_EOF;
		lexer->tokenizer = tokenize_null;
		return (1);
	}
	return (0);
}

static inline void	_set_normal_state(t_lex *lexer)
{
	lexer->state = NORMAL;
	lexer->tokenizer = tokenize_normal;
}

/* Changes state based on current ptr position */
int	do_state_transition(t_lex *lexer)
{
	debug_print("do_state_transition\n");
	if (!lexer->ptr)
		return (0);
	if (_handle_eof_state(lexer))
		return (0);
	if (_handle_quote_state(lexer))
		return (0);
	if (_handle_null_state(lexer))
		return (0);
	_set_normal_state(lexer);
	return (0);
}
