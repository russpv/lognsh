/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_do_arggrp.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rpeavey <rpeavey@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/10 16:33:32 by rpeavey           #+#    #+#             */
/*   Updated: 2025/04/10 16:33:33 by rpeavey          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parse_int.h"

// Modifies args token list
static int	_do_expansions(t_state *s, t_arg_data *grparg)
{
	int	res;

	if (!grparg || !s)
		return (ERR_ARGS);
	if (grparg->do_expansion)
	{
		res = lstiter_state(s, grparg->lst_tokens, tok_do_expansion);
		if (0 != res)
			return (res);
		tok_print_list(grparg->lst_tokens);
	}
	lgprint("Expansions Done.\n");
	return (0);
}

// Modifies args token list
static int	_do_wordsplits(t_state *s, t_arg_data *grparg)
{
	if (!grparg || !s)
		return (ERR_ARGS);
	if (grparg->do_expansion && !grparg->in_dquotes)
	{
		ft_lstiter_ins_rwd_tmp(get_mem(s), &grparg->lst_tokens,
			tok_do_wordsplits);
		tok_print_list(grparg->lst_tokens);
	}
	lgprint("Wordsplits Done.\n");
	return (0);
}

// Saves list in state
static int	_do_globbing(t_state *s, t_arg_data *grparg)
{
	t_list	**saved_lst;

	if (!grparg || !s)
		return (ERR_ARGS);
	saved_lst = get_tmp_tok_list(s);
	if (grparg->do_globbing || grparg->do_expansion)
	{
		ft_lstiter_ins_rwd_tmp(get_mem(s), saved_lst, p_do_globbing_toks);
	}
	lgprint("Globbing Done.\n");
	return (0);
}

// Saves modified token list in state cache
static int	_do_combine(t_state *s, t_arg_data *grparg)
{
	int		res;
	char	*str;
	t_list	**tok_lst;

	res = lstiter_state(s, grparg->lst_tokens, tok_do_grp_combine);
	if (0 != res)
		return (err("grp tok aggregation failure\n"), res);
	tok_lst = get_tmp_tok_list(s);
	str = get_tmp(s);
	if (str)
	{
		if (*str)
			ft_lstadd_back(tok_lst, ft_lstnew_tmp(get_mem(s),
					create_tmp_token(get_mem(s), str)));
		tok_print_list(*tok_lst);
	}
	lgprint("Combines and Inserts Done.\n");
	return (0);
}

/* Passed to arg llist iterator to iterate any group arg's token llist.
 * If (null) results, still returns empty heap string,
 * since a grouparg implies non-null word.
 * Ignores all (null) token raws.
 * Inserts into arg llist as needed.
 * expand > split > glob > combine > insert
 * // Explicit empty strings retained as empty strings.
// Unquoted nulls resulting from expansions are removed.
// Quoted nulls resulting from expansions are retained as empty strings,
//	(done prior)
// unless part of a non-null expansion word. -d'' is -d
 */
int	p_do_grparg_processing(t_state *s, t_list **this_arg, void *c)
{
	t_arg_data	*grparg;
	int			res;

	grparg = (t_arg_data *)c;
	if (NULL == c || false == grparg->is_grouparg || NULL == grparg->lst_tokens)
		return (0);
	dvprint(_MOD_ ": %s: got list: %p exp:%d glob:%d\n", __FUNCTION__,
		grparg->lst_tokens, grparg->do_expansion, grparg->do_globbing);
	res = _do_expansions(s, grparg);
	if (0 != res)
		return (res);
	res = _do_wordsplits(s, grparg);
	if (0 != res)
		return (res);
	res = _do_combine(s, grparg);
	if (0 != res)
		return (res);
	res = _do_globbing(s, grparg);
	if (0 != res)
		return (res);
	do_grparg_inserts(s, this_arg);
	dprint("%s: returning lst_size: %d, %p\n", __FUNCTION__,
		ft_lstsize(*this_arg), *this_arg);
	return (res);
}
