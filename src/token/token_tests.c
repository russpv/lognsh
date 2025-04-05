/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   token_tests.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dayeo <dayeo@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/05 09:10:25 by dayeo             #+#    #+#             */
/*   Updated: 2025/04/05 09:21:04 by dayeo            ###   ########.fr       */
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
