/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   token_add.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rpeavey <rpeavey@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/10 16:35:08 by rpeavey           #+#    #+#             */
/*   Updated: 2025/04/12 14:11:49 by rpeavey          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "token_int.h"

int	tok_add_subtok(t_mem_mgr *mgr, t_tok *grp, t_tok *sub)
{
	if (!grp || !sub)
		return (ERR_ARGS);
	ft_lstadd_back(&grp->t.meta.tokens, ft_lstnew_tmp(mgr, sub));
	return (0);
}
