/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_redir_normal.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rpeavey <rpeavey@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/10 16:34:04 by rpeavey           #+#    #+#             */
/*   Updated: 2025/04/10 16:34:05 by rpeavey          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parse_int.h"

#define EMSG_REDIR_SYM_MALLOC "Allocation for redirection symbol failed\n"
#define MSG_EOF "Expected a filename after redirection operator, got EOF\n"
#define MSG_INV "Expected a valid filename after redirection operator\n"
#define EMSG_REDIR_FN "Allocation for redirection filename failed\n"

static void	_set_tok(t_parser *p, t_redir_data *red, t_ast_node *n,
		t_tok *tok_fname)
{
	red->do_globbing = tok_get_globbing((t_tok *)tok_fname);
	red->do_expansion = tok_get_expansion((t_tok *)tok_fname);
	red->target_ptr = n;
	p_set_do_redir_expansion(n, red->do_expansion);
	p_set_do_redir_globbing(n, red->do_globbing);
	red->filename = ft_strdup_tmp(p->mmgr, tok_get_raw((t_tok *)tok_fname));
	if (!red->filename)
		exit_clean(&p->mmgr->list, ENOMEM, __FUNCTION__, EMSG_REDIR_FN);
}

static void	_set_grptok(t_parser *p, t_redir_data *red, t_ast_node *n,
		t_tok *tok_fname)
{
	if (is_group_token(tok_fname))
	{
		dprint(_MOD_ ": %s: got group redir %p, node %p\n", __FUNCTION__,
			tok_get_tlist(tok_fname), n);
		red->is_groupred = true;
		p_set_has_redgrouptoks(n, 1);
		red->lst_tokens = ft_lstcopy_tmp(get_mem(p->global_state),
				tok_get_tlist(tok_fname), copy_token, destroy_token);
	}
}

static int	_test_tok(t_parser *p, t_tok *tok_fname)
{
	dprint("%s: %s: fname:%s_\n", _MOD_, __FUNCTION__, tok_get_raw(tok_fname));
	if (!(is_filename_token((t_tok *)tok_fname)
			|| is_expansion((t_tok *)tok_fname))
		|| 0 == ft_strcmp(tok_get_raw(tok_fname), ""))
	{
		print_redir_error(p->global_state,
			tok_get_raw((p->curr_tok)->prev->prev->content),
			tok_get_pos((p->curr_tok)->prev->prev->content));
		return (err(MSG_INV), ERR_SYNTAX);
	}
	return (0);
}

// Stores redirection symbol, globbing, expansion, filename, and advances parser
/* tok: current redirect token returned from advance()
 * Freeing handled by caller in case of error
 */
int	process_normal_redir(t_parser *p, t_tok *tok, t_redir_data *red,
		t_ast_node *n)
{
	t_tok	*tok_fname;

	if (!p || !red || !tok)
		return (ERR_ARGS);
	red->symbol = ft_strdup_tmp(p->mmgr, tok_get_raw((t_tok *)tok));
	if (!red->symbol)
		exit_clean(&p->mmgr->list, ENOMEM, __FUNCTION__, EMSG_REDIR_SYM_MALLOC);
	dprint("%s: %s: redir typ:%d sym:%s\n", _MOD_, __FUNCTION__, red->type,
		red->symbol);
	if (is_at_end(p))
	{
		print_parse_error(p->global_state,
			tok_get_raw((p->curr_tok)->prev->content),
			tok_get_pos((p->curr_tok)->content));
		return (err(MSG_EOF), ERR_SYNTAX);
	}
	tok_fname = advance(p);
	if (0 != _test_tok(p, tok_fname))
		return (ERR_SYNTAX);
	_set_grptok(p, red, n, tok_fname);
	_set_tok(p, red, n, tok_fname);
	return (0);
}
