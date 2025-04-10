/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lex_utils_tok_create.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rpeavey <rpeavey@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/10 16:33:04 by rpeavey           #+#    #+#             */
/*   Updated: 2025/04/10 16:33:05 by rpeavey          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "lex_int.h"

static void	_init_token(t_lex *lexer, t_tok *token)
{
	if (true == lexer->is_subtoken)
		tok_set_subtoken(token);
	if (false == lexer->do_wordsplit)
		tok_set_dquotes(token);
	if (true == lexer->do_globbing)
	{
		tok_set_globbing(token);
		tok_set_globbing(lexer->last_grp_tok);
	}
	if (true == lexer->do_expansion)
	{
		tok_set_expansion(token);
		tok_set_expansion(lexer->last_grp_tok);
	}
	lexer->do_globbing = INITVAL;
	lexer->do_expansion = INITVAL;
	dvprint(_MOD_ ": %s: Created token \n", __FUNCTION__);
}

/* Creates token based on current buf and ptr, does not add to llist
 * If buf is empty, creates grp token only.
 * Resets buf and buf_idx and tokenizing flags.
 * Looks ahead for normal delim to reset subtoken flag.
 * Note: skip any terminator that is not to be part of token (\")
 */
t_tok	*lex_create_token(t_mem_mgr *m, t_lex *lexer, int type)
{
	t_tok	*token;
	t_tok	*grp_token;

	dvprint(_MOD_ ": %s\n", __FUNCTION__);
	if (!lexer)
		return (NULL);
	if (true == lexer->is_subtoken && NULL == lexer->last_grp_tok)
	{
		grp_token = create_token(m, lexer->buf, TOK_GROUP_WORD,
				(size_t)(lexer->ptr - lexer->raw_string));
		if (!grp_token)
			exit_clean(&m->list, ENOMEM, __FUNCTION__, EMSG_MALLOC);
		lexer->last_grp_tok = grp_token;
		dprint(_MOD_ ": %s: Created GROUP token\n", __FUNCTION__);
	}
	if (0 == ft_strlen(lexer->buf))//TEST THAT THIS NULL TOKEN WORKS INSTEAD OF return (NULL);
		;
	token = create_token(m, lexer->buf, type, (size_t)(lexer->ptr
				- lexer->raw_string));
	if (token)
	{
		_init_token(lexer, token);
	}
	return (token);
}
