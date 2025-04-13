/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   token_.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rpeavey <rpeavey@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/10 16:35:07 by rpeavey           #+#    #+#             */
/*   Updated: 2025/04/13 13:58:17 by rpeavey          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "token_int.h"

#define DMSG_CT ": %s: %s_ typ_%02d len_%ld\n"

static void	_init_group_token(t_tok *token, size_t pos, int type)
{
	dprint(_MOD_ ": %s: (grp) typ_%02d \n", __FUNCTION__, type);
	token->class = GROUP;
	token->t.meta.pos = pos;
	token->t.meta.tokc = 0;
	token->t.meta.in_dquotes = false;
	token->t.meta.do_expansion = false;
	token->t.meta.do_globbing = false;
	token->t.meta.tokens = NULL;
}

static int	_init_normal_token(t_tok *token, int type, size_t pos)
{
	token->class = NORMAL;
	token->t.tok.type = type;
	token->t.tok.pos = pos;
	token->t.tok.is_subtoken = false;
	token->t.tok.do_expansion = false;
	token->t.tok.in_dquotes = false;
	token->t.tok.do_globbing = false;
	token->t.tok.is_combinable = true;
	return (0);
}

t_tok	*create_token(t_mem_mgr *mgr, const char *s, int type, size_t pos)
{
	t_tok	*token;

	if (!mgr)
		return (NULL);
	token = mgr->f(&mgr->list, sizeof(t_tok));
	if (token)
	{
		if (type == TOK_GROUP_WORD)
			_init_group_token(token, pos, type);
		else
		{
			token->t.tok.raw = NULL;
			token->t.tok.raw_len = ft_strnlen(s, MAX_INPUT_SZ);
			dprint(_MOD_ DMSG_CT, __FUNCTION__, s, type, token->t.tok.raw_len);
			if (token->t.tok.raw_len > MAX_INT_BUFLEN)
				return (pbufflow(s), mgr->dealloc(&mgr->list, token), NULL);
			if (token->t.tok.raw_len == 0 && TOK_NAME == type \
				&& TOK_WORD == type)
				type = TOK_EMPTY;
			else
				token->t.tok.raw = ft_strdup_tmp(mgr, s);
			_init_normal_token(token, type, pos);
		}
	}
	return (token);
}

// Creates a simple token, non-combinable
// Signals no further processing needed
void	*create_tmp_token(t_mem_mgr *mgr, const void *s)
{
	t_tok	*token;

	if (!s)
		return (NULL);
	token = mgr->f(&mgr->list, sizeof(t_tok));
	if (token)
	{
		dvprint(_MOD_ ": %s: %s_ typ_%d \n", __FUNCTION__, (const char *)s,
			TOK_WORD);
		if (ft_strnlen(s, MAX_INPUT_SZ) > MAX_INT_BUFLEN)
			return (pbufflow(s), mgr->dealloc(&mgr->list, token), NULL);
		token->t.tok.raw = ft_strdup_tmp(mgr, s);
		if (!token->t.tok.raw)
			exit_clean(&mgr->list, ENOMEM, __FUNCTION__, EMSG_MALLOC);
		_init_normal_token(token, TOK_WORD, -1);
		token->t.tok.is_combinable = false;
	}
	return (token);
}

// Creates a dummy token as result of expansion
// Signals combiner to combine only token list boundaries
void	*create_split_token(t_mem_mgr *mgr, const void *s)
{
	t_tok	*token;

	if (!s)
		return (NULL);
	token = mgr->f(&mgr->list, sizeof(t_tok));
	if (token)
	{
		dprint(_MOD_ ": %s: %s_ typ_%d \n", __FUNCTION__, (const char *)s,
			TOK_WORD);
		if (ft_strnlen(s, MAX_INPUT_SZ) > MAX_INT_BUFLEN)
			return (pbufflow(s), mgr->dealloc(&mgr->list, token), NULL);
		token->t.tok.raw = ft_strdup_tmp(mgr, s);
		if (!token->t.tok.raw)
			exit_clean(&mgr->list, ENOMEM, __FUNCTION__, EMSG_MALLOC);
		_init_normal_token(token, TOK_WORD, -1);
		token->t.tok.is_combinable = false;
		token->t.tok.do_expansion = true;
	}
	return (token);
}
