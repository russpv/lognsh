/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   token_getters.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rpeavey <rpeavey@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/10 16:35:11 by rpeavey           #+#    #+#             */
/*   Updated: 2025/04/10 16:35:12 by rpeavey          ###   ########.fr       */
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
		return (GRP_TOK_RAW);
	return (token->t.tok.raw);
}

enum e_tok_type	tok_get_type(t_tok *token)
{
	if (GROUP == token->class)
		return (TOK_GROUP_WORD);
	return (token->t.tok.type);
}

size_t	tok_get_len(t_tok *token)
{
	if (!token)
		return (-1);
	if (NORMAL == token->class)
		return (token->t.tok.raw_len);
	return (-1);
}
