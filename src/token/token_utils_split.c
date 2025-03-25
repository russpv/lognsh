#include "token_int.h"


char	*_skip_delims(char const *s, char const *ref)
{
	char *delims;
	char *word;

	if (!s)
		return (NULL);
	word = (char *)s;
	while (word && *word)
	{
		delims = (char *)ref;
		while (*delims)
		{
			if (*word == *delims)
				break;
			delims++;
		}
		if (0 == *delims)
			return (word);
		word++;
	}
	return (NULL);
}

// Returns list of strings
// Leading spaces are preserved!
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
	debug_print(_MOD_ ": %s: Got _%s_\n", __FUNCTION__, word);
	if (NULL != ft_strchrs(_skip_delims(word, IFS), IFS))
	{
		debug_print("splitting\n");
		split_raw = ft_split_ifs_mem(&mem, word, IFS);
		while (split_raw[++i])
		{
			new = ft_lstnew_tmp(mgr, split_raw[i]);
			if (NULL == new)
				exit_clean(&mgr->list, ENOMEM, __FUNCTION__, EMSG_MALLOC);
			ft_lstadd_back(&lst, new);
		}
	}
	return (lst);
}

/*
 * ins_lst is incoming list of split strings to be inserted
 * at the lst_pos position in a grparg token list, overwriting it.
 * Deletes lst_pos, deep copies insert list and inserts
 * at lst_pos position.
 * ins_lst is then destroyed.
 * Sets is_combinable on the first token.
 */
int	do_tok_inserts(t_mem_mgr *mgr, t_list **lst_pos, t_list **ins_lst)
{
	t_list	*head;
	t_list	*new_lst;
	t_tok	*new_tok;

	new_lst = NULL;
	new_tok = NULL;
	if (ft_lstsize(*ins_lst) > 1)
	{
		debug_print(_MOD_ ": %s: Inserting lst \n", __FUNCTION__);
		new_lst = ft_lstmap_tmp(mgr, *ins_lst, create_tmp_token, destroy_token);
		((t_tok*)((ft_lstfirst(new_lst))->content))->t.tok.is_combinable = true;
		ft_lstadd_insert(lst_pos, new_lst);
		head = ft_lstfirst(*lst_pos);
		ft_lstdelone_rwd_tmp(mgr, &head, lst_pos, destroy_token);
	}
	else
	{
		debug_print(_MOD_ ": %s: Inserting single tok _%s_\n", __FUNCTION__,\
			(*ins_lst)->content);
		new_tok = create_tmp_token(mgr, (*ins_lst)->content);
		if (!new_tok)
			exit_clean(&mgr->list, ENOMEM, __FUNCTION__, EMSG_MALLOC);
		tok_print(new_tok);
		fprintf(stderr, "HEYYYY\n");
		new_tok->t.tok.is_combinable = true;
		destroy_token(mgr, (*lst_pos)->content);
		(*lst_pos)->content = new_tok;
	}
	ft_lstclear_str_tmp(mgr, ins_lst);
	return (0);
}

#define DBGMSG_MATCHES _MOD_ ": %s split %d words, 1st: %s\n"

// Takes list of tokens (lst_pos) and returns list of strings.
// Returns list of strings
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
		str_lst = split_word(mgr, tok->t.tok.raw); //dont trim
		if (str_lst)
		{
			debug_print(DBGMSG_MATCHES, __FUNCTION__, ft_lstsize(str_lst),
			str_lst->content);
			do_tok_inserts(mgr, lst_pos, &str_lst);
			ft_lstclear_str_tmp(mgr, &str_lst);
			tok_print_list(*lst_pos);
			debug_print(_MOD_ ": %s: Done.\n", __FUNCTION__);
		}
		else
			debug_print(_MOD_ ": %s: Nothing split or inserted.\n",
				__FUNCTION__);
	}
	return (0);
}
