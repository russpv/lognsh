
#include "lex_int.h"

/*
** Conditions for exiting DOLLAR mode
** - ... (end of valid name)
**
** These are the conditions for tokenizing full tokens
** - ...&& not ...&
** - ...|[|]
** - ...<* >*
** - ...( or )
** - ...\n or \t or ' '
**
** These are the conditions for tokenizing subtokens
** - ...\' ...\'
** - ...\" ...\"
** - ...$? and ...$[varname]
**
** Any state must stop on tokenizing delimiters.
** State transitions otherwise imply subtokens unless
** followed immediately by another tokenizing delimiter.
*/

static void	_init_lexer_state(const char *input, enum e_lex_state *state)
{
	if ((unsigned char)OP_SQUOTE == *input)
		*state = IN_SINGLE_QUOTES;
	else if ((unsigned char)OP_DQUOTE == *input)
		*state = IN_DOUBLE_QUOTES;
	else
		*state = NORMAL;
}

static void	_err_or_sigint(t_state *s, int exitcode)
{
	if (ERR_CHILD_SIGINT == exitcode)
		set_exit_status(s, ERR_CHILD_SIGINT);
	else
		set_exit_status(s, EX_ERNDM);
}

t_lex	*tokenize(t_state *s, const char *input)
{
	enum e_lex_state	state;
	t_lex				*lexer;
	int					res;

	_init_lexer_state(input, &state);
	lexer = create_lexer(s, state, input);
	if (lexer)
	{
		while (DONE != lexer->state)
		{
			res = lexer->tokenizer(s, lexer);
			if (0 != res)
				return (destroy_lexer(get_mem(s), (void **)&lexer),
					dprint(_MOD_ ": ERR\n"), _err_or_sigint(s, res),
					NULL);
			do_state_transition(lexer);
		}
	}
	debug_detect_cycle(lexer->token_list);
	lex_print(lexer);
	return (lexer);
}
