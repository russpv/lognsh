/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   token_tests.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rpeavey <rpeavey@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/10 16:35:14 by rpeavey           #+#    #+#             */
/*   Updated: 2025/04/10 16:35:15 by rpeavey          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "token_int.h"

bool	tok_get_issubtoken(t_tok *token)
{
	if (!token)
		return (false);
	if (GROUP == token->class)
		return (false);
	return (true == token->t.tok.is_subtoken);
}

bool	tok_isgrouptoken(t_tok *token)
{
	if (!token)
		return (false);
	return (GROUP == token->class);
}
