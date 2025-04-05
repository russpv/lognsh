/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   token_utils_split.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dayeo <dayeo@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/05 09:10:45 by dayeo             #+#    #+#             */
/*   Updated: 2025/04/05 09:23:23 by dayeo            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "token_int.h"

char	*_skip_delims_post(char const *s, char const *ref)
{
	char	*delims;
	char	*end;
	int		len;

	if (!s)
		return (NULL);
	len = ft_strnlen((char *)s, MAX_ENVVAR_LEN);
	if (len < 2)
		return (NULL);
	end = (char *)s + len - 1;
	while (end != (char *)s)
	{
		delims = (char *)ref;
		while (*delims)
		{
			if (*end == *delims)
				break ;
			delims++;
		}
		if (0 == *delims)
			return (end);
		end--;
	}
	return (NULL);
}

char	*_skip_delims(char const *s, char const *ref)
{
	char	*delims;
	char	*word;

	if (!s)
		return (NULL);
	word = (char *)s;
	while (word && *word)
	{
		delims = (char *)ref;
		while (*delims)
		{
			if (*word == *delims)
				break ;
			delims++;
		}
		if (0 == *delims)
			return (word);
		word++;
	}
	return (NULL);
}

/*
int	do_tok_inserts(t_mem_mgr *mgr, t_list **lst_node, t_list **ins_lst,
	void *tok)
{
	t_list	*new_tok_lst;
	char	*new_tok;
	t_tok *content;

	new_tok_lst = NULL;
	new_tok = NULL;
	content = (t_tok *)tok;
	if (ft_lstsize(*ins_lst) > 1)
	{
		ft_lstdelone_rwd_tmp(mgr, lst_node, lst_node, destroy_token);
		new_tok_lst = ft_lstmap_tmp(mgr, *ins_lst, create_tmp_token,
				destroy_token);
		ft_lstadd_insert(lst_node, new_tok_lst);
		ft_lstclear_str_tmp(mgr, ins_lst);
	}
	else // one token, don't delete node, just replace content
	{
		new_tok = copy_token(mgr, content);
		if (!new_tok)
			exit_clean(&mgr->list, ENOMEM, __FUNCTION__, EMSG_MALLOC);
		destroy_token(mgr, content);
		content = new_tok;
		ft_lstclear_str_tmp(mgr, ins_lst);
	}
	return (0);
}*/

/*
 * ins_lst is incoming list of split strings to be inserted
 * at the lst_pos position in a grparg token list, overwriting it.
 * Deletes lst_pos, deep copies insert list and inserts
 * at lst_pos position.
 * ins_lst is then destroyed.
 * Sets is_combinable on the first token. (Use with create_tmp_token)
 */

//#define DBGMSG_MATCHES _MOD_ ": %s split %d words, 1st: %s\n"

// Takes list of tokens (lst_pos) and inserts list of split tokens 
// at lst_pos node
int	tok_do_wordsplits(t_mem_mgr *mgr, t_list **lst_pos, void *lst_c)
{
	t_list		*str_lst;
	const t_tok	*tok = (t_tok *)lst_c;

	str_lst = NULL;
	assert(NORMAL == tok->class);
	debug_print(_MOD_ ": %s: Got _%s_\n", __FUNCTION__, tok->t.tok.raw);
	tok_print_list(*lst_pos);
	if (false == tok->t.tok.in_dquotes)
	{
		str_lst = split_word(mgr, tok->t.tok.raw);
		if (str_lst)
		{
			do_tok_inserts(mgr, lst_pos, &str_lst, create_split_token);
			ft_lstclear_str_tmp(mgr, &str_lst);
			debug_print(_MOD_ ": %s: Done.\n", __FUNCTION__);
		}
		else
			debug_print(_MOD_ ": %s: Nothing split or inserted.\n",
				__FUNCTION__);
	}
	return (0);
}
