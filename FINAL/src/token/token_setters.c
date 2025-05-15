/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   token_setters.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rpeavey <rpeavey@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/10 16:35:13 by rpeavey           #+#    #+#             */
/*   Updated: 2025/04/10 16:35:14 by rpeavey          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "token_int.h"

int	tok_set_globbing(t_tok *token)
{
	if (!token)
		return (ERR_ARGS);
	if (GROUP == token->class)
		return (ERR_ARGS);
	token->t.tok.do_globbing = true;
	dprint(_MOD_ ": %s: %s_ glob_%d \n", __FUNCTION__, tok_get_raw(token),
		tok_get_globbing(token));
	return (0);
}

int	tok_set_expansion(t_tok *token)
{
	if (!token)
		return (ERR_ARGS);
	if (GROUP == token->class)
		token->t.meta.do_expansion = true;
	else
		token->t.tok.do_expansion = true;
	return (0);
}

int	tok_set_dquotes(t_tok *token)
{
	if (!token)
		return (ERR_ARGS);
	if (GROUP == token->class)
		return (ERR_ARGS);
	token->t.tok.in_dquotes = true;
	return (0);
}

int	tok_set_subtoken(t_tok *token)
{
	if (!token)
		return (ERR_ARGS);
	if (GROUP == token->class)
		return (ERR_ARGS);
	token->t.tok.is_subtoken = true;
	return (0);
}

int	tok_incr_tokc(t_tok *token)
{
	if (!token)
		return (ERR_ARGS);
	if (GROUP != token->class)
		return (ERR_ARGS);
	token->t.meta.tokc++;
	return (0);
}
