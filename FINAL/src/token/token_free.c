/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   token_free.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rpeavey <rpeavey@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/10 16:35:10 by rpeavey           #+#    #+#             */
/*   Updated: 2025/04/10 16:35:11 by rpeavey          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "token_int.h"

/* void star star due to linked list destroy method */
void	destroy_token(t_mem_mgr *mgr, void **token)
{
	t_tok	*t;

	if (!token)
		return ;
	if (!(*token))
		return ;
	t = (t_tok *)(*token);
	if (GROUP == t->class)
		ft_lstclear_tmp(mgr, &t->t.meta.tokens, destroy_token);
	else
	{
		if (t->t.tok.raw)
			mgr->dealloc(&mgr->list, t->t.tok.raw);
	}
	mgr->dealloc(&mgr->list, t);
	*token = NULL;
}
