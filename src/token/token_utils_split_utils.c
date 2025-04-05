/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   token_utils_split_utils.c                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dayeo <dayeo@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/05 19:46:22 by dayeo             #+#    #+#             */
/*   Updated: 2025/04/05 19:46:34 by dayeo            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "token_int.h"

// Returns list of strings
// Leading and lagging spaces are preserved!
t_list	*split_word(t_mem_mgr *mgr, const char *word)
{
	t_list				*lst;
	t_list				*new;
	char				**split_raw;
	struct s_mem_utils	mem;
	int					i;

	if (!word || !mgr)
		return (NULL);
	mem_struct_init(mgr, &mem);
	lst = NULL;
	i = -1;
	if (NULL != ft_strchrs_betw(_skip_delims(word, IFS), \
		_skip_delims_post(word, IFS), IFS))
	{
		split_raw = ft_split_ifs_mem(&mem, word, IFS);
		while (split_raw[++i])
		{
			new = ft_lstnew_tmp(mgr, split_raw[i]);
			if (NULL == new)
				exit_clean(&mgr->list, ENOMEM, __FUNCTION__, EMSG_MALLOC);
			ft_lstadd_back(&lst, new);
		}
		ft_lstprint(lst);
	}
	return (lst);
}

static int	_insert_multi_nodes(t_mem_mgr *mgr, t_list **lst_pos, \
			t_list **ins_lst, \
			void *(*createf)(t_mem_mgr *, const void *))
{
	t_list	*head;
	t_list	*new_lst;
	bool	reattach_head;

	new_lst = ft_lstmap_tmp(mgr, *ins_lst, createf, destroy_token);
	((t_tok *)((ft_lstfirst(new_lst))->content))->t.tok.is_combinable = true;
	ft_lstadd_insert(lst_pos, new_lst);
	head = ft_lstfirst(*lst_pos);
	reattach_head = (head == *lst_pos);
	ft_lstdelone_rwd_tmp(mgr, &head, lst_pos, destroy_token);
	if (reattach_head)
		*lst_pos = head;
	return (0);
}

int	do_tok_inserts(t_mem_mgr *mgr, t_list **lst_pos, t_list **ins_lst, \
		void *(*createf)(t_mem_mgr *, const void *))
{
	t_tok	*new_tok;

	if (ft_lstsize(*ins_lst) > 1)
	{
		_insert_multi_nodes(mgr, lst_pos, ins_lst, createf);
	}
	else
	{
		new_tok = createf(mgr, (*ins_lst)->content);
		if (!new_tok)
			exit_clean(&mgr->list, ENOMEM, __FUNCTION__, EMSG_MALLOC);
		tok_print(new_tok);
		new_tok->t.tok.is_combinable = true;
		destroy_token(mgr, &(*lst_pos)->content);
		(*lst_pos)->content = new_tok;
	}
	ft_lstclear_str_tmp(mgr, ins_lst);
	return (0);
}

/*int	do_tok_inserts(t_mem_mgr *mgr, t_list **lst_pos, t_list **ins_lst, \
	void *(*createf)(t_mem_mgr *, const void *))
{
	t_list	*head;
	t_list	*new_lst;
	t_tok	*new_tok;
	bool	reattach_head;

	new_lst = NULL;
	new_tok = NULL;
	reattach_head = false;
	if (ft_lstsize(*ins_lst) > 1)
	{
		new_lst = ft_lstmap_tmp(mgr, *ins_lst, createf, destroy_token);
		((t_tok *)((ft_lstfirst(new_lst))->content))->t.tok.is_combinable \
			= true;
		ft_lstadd_insert(lst_pos, new_lst);
		head = ft_lstfirst(*lst_pos);
		if (head == *lst_pos)
			reattach_head = true;
		ft_lstdelone_rwd_tmp(mgr, &head, lst_pos, destroy_token);
		if (reattach_head)
			*lst_pos = head;
	}
	else
	{
		new_tok = createf(mgr, (*ins_lst)->content);
		if (!new_tok)
			exit_clean(&mgr->list, ENOMEM, __FUNCTION__, EMSG_MALLOC);
		tok_print(new_tok);
		new_tok->t.tok.is_combinable = true;
		destroy_token(mgr, &(*lst_pos)->content);
		(*lst_pos)->content = new_tok;
	}
	ft_lstclear_str_tmp(mgr, ins_lst);
	return (0);
}*/
