/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lex_utils_tok_add.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rpeavey <rpeavey@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/10 16:33:03 by rpeavey           #+#    #+#             */
/*   Updated: 2025/04/12 19:43:26 by rpeavey          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "lex_int.h"

static int	_add_subtoken(t_mem_mgr *m, t_lex *lexer, t_tok *subtok)
{
	tok_add_subtok(m, lexer->last_grp_tok, subtok);
	tok_incr_tokc(lexer->last_grp_tok);
	return (0);
}

int	add_grptoken(t_mem_mgr *m, t_lex *lexer)
{
	dprint(_MOD_ ": %s adding token (grp) to list\n", __FUNCTION__);
	ft_lstadd_back(&lexer->token_list, ft_lstnew_tmp(m, lexer->last_grp_tok));
	lexer->is_subtoken = false;
	lexer->last_grp_tok = NULL;
	lexer->is_assignment = false;
	lexer->tokc++;
	return (0);
}

static void	_commit_token_or_subtoken(t_mem_mgr *m, t_lex *lexer, t_tok *token)
{
	if (lexer->last_grp_tok)
	{
		dprint(_MOD_ ": %s adding subtoken %s to grp\n", __FUNCTION__,
			tok_get_raw(token));
		_add_subtoken(m, lexer, token);
		dprint(_MOD_ ": %s checking delimiter, ptr:%c\n", __FUNCTION__,
			*lexer->ptr);
		if (NORMAL == lexer->state && is_normal_delim(lexer, 0))
		{
			dprint(_MOD_ ": %s adding grptoken\n", __FUNCTION__);
			add_grptoken(m, lexer);
		}
	}
	else
	{
		dprint(_MOD_ ": %s adding token to list\n", __FUNCTION__);
		if (ft_lstadd_back(&lexer->token_list, ft_lstnew_tmp(m,
					token)) > 0)
			lexer->tokc++;
		lexer->is_assignment = false;
	}
}

/* Adds to llist tail a new token, clears buf */
int	add_token(t_mem_mgr *m, t_lex *lexer, t_tok *token)
{
	dvprint(_MOD_ ": %s\n", __FUNCTION__);
	if (token && lexer)
	{
		if (lexer->lex_err)
			return (ERR_GENERAL);
		ft_memset(lexer->buf, 0, LEX_BUFSZ);
		lexer->buf_idx = 0;
		if (lexer->tokc < LEX_MAX_TOKC)
		{
			_commit_token_or_subtoken(m, lexer, token);
		}
		else
			return (ERR_BUFFLOW);
		return (0);
	}
	return (ERR_ARGS);
}
