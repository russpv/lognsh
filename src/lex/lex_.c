/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lex_.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dayeo <dayeo@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/05 11:50:04 by dayeo             #+#    #+#             */
/*   Updated: 2025/04/05 20:49:52 by dayeo            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

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
				destroy_lexer(get_mem(s), (void **)&lexer);
				return (debug_print(_MOD_ ": ERR\n"), NULL);
			}
			do_state_transition(lexer);
		}
	}
	debug_detect_cycle(lexer->token_list);
	lex_print(lexer);
	return (lexer);
}
