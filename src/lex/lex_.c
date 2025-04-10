/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lex_.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rpeavey <rpeavey@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/10 16:32:19 by rpeavey           #+#    #+#             */
/*   Updated: 2025/04/10 16:32:20 by rpeavey          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "lex_int.h"

/*
** These are the conditions for exiting DOLLAR mode
** ... (end of valid name)
** 
** These are the conditions for tokenizing full tokens
** ...&& not ...&
** ...|[|] 
** ...<* ...>* 
** ...( or )  
** ...\n or \t or ' ' 
** 
** These are the conditions for tokenizing subtokens
** ...\' ...\'
** ...\" ...\"
** ...$?
** ...$[varname]
** 
** Any of the states must stop on the tokenizing delimiters
** All state transitions otherwise signify subtokens unless 
** a tokenizing delimiter also follows.
*/

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
				destroy_lexer(get_mem(s), (void **)&lexer);
				return (dprint(_MOD_ ": ERR\n"), NULL);
			}
			do_state_transition(lexer);
		}
	}
	debug_detect_cycle(lexer->token_list);
	lex_print(lexer);
	return (lexer);
}
