#include "lex_int.h"

// assumes non-empty string
t_lex	*tokenize(t_state *s, const char *input)
{
	enum e_lex_state	state;
	t_lex				*lexer;

	if ((unsigned char)OP_SQUOTE == *input)
		state = IN_SINGLE_QUOTES;
	else if ((unsigned char)OP_DQUOTE == *input)
		state = IN_DOUBLE_QUOTES;
	else
		state = NORMAL;
	lexer = create_lexer(s, state, input);
	if (lexer)
	{
		while (DONE != lexer->state)
		{
			if (0 != lexer->tokenizer(s, lexer))
			{
				destroy_lexer(s, lexer);
				return (debug_print(_MOD_": ERR\n"), NULL);
			}
			do_state_transition(lexer);
		}
	}
	debug_detect_cycle(lexer->token_list);
	lex_print(lexer);
	return (lexer);
}
