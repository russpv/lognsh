/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   token_setters_utils.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dayeo <dayeo@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/05 20:08:39 by dayeo             #+#    #+#             */
/*   Updated: 2025/04/05 20:08:48 by dayeo            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "token_int.h"

int	tok_incr_tokc(t_tok *token)
{
	if (!token)
		return (ERR_ARGS);
	if (GROUP != token->class)
		return (ERR_ARGS);
	token->t.meta.tokc++;
	return (0);
}

int	tok_add_subtok(t_mem_mgr *mgr, t_tok *grp, t_tok *sub)
{
	if (!grp || !sub)
		return (ERR_ARGS);
	ft_lstadd_back(&grp->t.meta.tokens, ft_lstnew_tmp(mgr, sub));
	return (0);
}
