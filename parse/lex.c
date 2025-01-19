#include "lex_int.h"

t_lex	*create_lexer(int start_state)
{
	t_lex *lexer = malloc(sizeof(t_lex));
	if (lexer)
	{
		lexer->state = start_state;
		if (NORMAL == start_state)
			lexer->tokenizer = tokenize_normal;
		if (IN_SINGLE_QUOTES == start_state)
			lexer->tokenizer = tokenize_single_quotes;
		if (IN_DOUBLE_QUOTES == start_state)
			lexer->tokenizer = tokenize_double_quotes;
		lexer->buf = malloc(sizeof(char) * LEX_BUFSZ);
		if (!lexer->buf)
		{
			free(lexer);
			lexer = NULL;
		}
		lexer->hasht = ht_create();
		if (!lexer->hasht)
		{
			free(lexer);
			free(lexer->buf);
			lexer = NULL;
		}
	}
	return (lexer);
}

void	destroy_lexer(t_lex *lexer)
{
	if (!lexer)
		return ;
	free(lexer->buf);
	ft_lstclear(lexer->token_list, destroy_token);
	free(lexer);
	lexer = NULL;
}

static int	_do_state_transition(t_lex *lexer)
{
	if (!lexer->ptr || !*(++lexer->ptr))
		lexer->state = DONE;
	if (OP_DQUOTE == *lexer->ptr)
	{
		lexer->state = IN_DOUBLE_QUOTES;
		lexer->tokenizer = tokenize_double_quotes;
	}
	else if ((unsigned char)*lexer->ptr == OP_SQUOTE)
	{
		lexer->state = IN_SINGLE_QUOTES;
		lexer->tokenizer = tokenize_single_quotes;
	}
	else
	{
		lexer->state = NORMAL;
		lexer->tokenizer = tokenize_normal;
	}
	return (0);
}

// assumes non-empty string
t_lex	*tokenize(const char *input)
{
	e_lex_state state;

	if ((unsigned char)OP_SQUOTE == *input)
		state = IN_SINGLE_QUOTES;
	else if ((unsigned char)OP_DQUOTE == *input)
		state = IN_DOUBLE_QUOTES;
	else
		state = NORMAL;
	t_lex *lexer = create_lexer(state);
	if (lexer)
	{
		while (DONE != lexer->state)
		{
			if (1 == lexer->tokenizer(lexer))
			{
				destroy_lexer(lexer);
				break ;
			}
			_do_state_transition(lexer);
		}
	}
	return (lexer);
}
int	build_hasht(t_lex *lexer)
{
	//TODO
	(void)lexer;
	return (0);
	/* add token defs and substring flags */
}
