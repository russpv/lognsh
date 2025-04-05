/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   token_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dayeo <dayeo@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/05 09:10:51 by dayeo             #+#    #+#             */
/*   Updated: 2025/04/05 09:24:20 by dayeo            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "token_int.h"

int	tok_print(void *content)
{
	t_tok	*tok;

	tok = (t_tok *)content;
	if (tok)
	{
		if (GROUP == tok->class)
		{
			log_print(_MOD_ ": Grp_tok:(grp) Exp:- Glb:- Sub:- tokc:%d \n",
				tok->t.meta.tokc);
			if (0 != ft_lstiter(tok->t.meta.tokens, tok_print))
				err("LSTITER");
		}
		else if (tok_get_issubtoken(tok))
			log_print(_MOD_ ": Sub_tok:%02d Exp:%d Glb:%d Sub:%d Dq:%d +:%d Val:(%s)\
				 \n",
				tok->t.tok.type, tok->t.tok.do_expansion,
				tok->t.tok.do_globbing, tok->t.tok.is_subtoken,
				tok->t.tok.in_dquotes, tok->t.tok.is_combinable, \
					tok->t.tok.raw);
		else
			log_print(_MOD_ ": Token:%02d Exp:%d Glb:%d Sub:%d Dq:%d +:%d Val:(%s)\
				\n", tok->t.tok.type, tok->t.tok.do_expansion,
				tok->t.tok.do_globbing, tok->t.tok.is_subtoken,
				tok->t.tok.in_dquotes, tok->t.tok.is_combinable, \
					tok->t.tok.raw);
	}
	return (0);
}

void	tok_print_list(t_list *head)
{
	t_list	*current;
	int		i;

	current = head;
	i = 0;
	if (DEBUG && isatty(STDOUT_FILENO))
	{
		while (current)
		{
			debug_print("Node %d:\n", i);
			tok_print(current->content);
			current = current->next;
			i++;
		}
		debug_print("List printed.\n");
	}
}
