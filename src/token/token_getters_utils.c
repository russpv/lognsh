/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   token_getters_utils.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dayeo <dayeo@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/05 20:14:29 by dayeo             #+#    #+#             */
/*   Updated: 2025/04/05 20:14:42 by dayeo            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "token_int.h"

size_t	tok_get_pos(t_tok *token)
{
	if (!token)
		return (-1);
	if (GROUP == token->class)
		return (token->t.meta.pos);
	else
		return (token->t.tok.pos);
}

// Returns list of subtokens, if a group token
t_list	*tok_get_tlist(t_tok *token)
{
	if (!token)
		return (NULL);
	if (GROUP != token->class)
		return (NULL);
	return (token->t.meta.tokens);
}

char	*tok_get_raw(t_tok *token)
{
	if (!token)
		return (NULL);
	if (GROUP == token->class)
	{
		return (NULL);
	}
	return (token->t.tok.raw);
}

size_t	tok_get_len(t_tok *token)
{
	if (!token)
		return (-1);
	if (NORMAL == token->class)
		return (token->t.tok.raw_len);
	return (-1);
}
