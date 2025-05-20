#include "tokens_int.h"


/* -------------------------------------------------------------------------
 * Function Group: Token word splitting logic
 * Description   : 
 * ------------------------------------------------------------------------- */

char	*_skip_delims_post(char const *s, char const *ref)
{
	char	*delims;
	char	*end;
	int		len;

	if (!s)
		return (NULL);
	len = ft_strnlen((char *)s, MAX_NAME_LEN);
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

static t_list	*_do_split(t_mem_mgr *mgr, const char *word)
{
	t_list				*lst;
	t_list				*new;
	struct s_mem_utils	mem;
	char				**split_raw;
	int					i;

	lst = NULL;
	mem_struct_init(mgr, &mem);
	split_raw = ft_split_ifs_mem(&mem, word, IFS);
	i = -1;
	while (split_raw[++i])
	{
		new = ft_lstnew_tmp(mgr, split_raw[i]);
		if (NULL == new)
			exit_clean(&mgr->list, ENOMEM, __FUNCTION__, EMSG_MALLOC);
		ft_lstadd_back(&lst, new);
	}
	return (lst);
}

// Returns list of strings
// Leading and lagging spaces are preserved!
t_list	*split_word(t_mem_mgr *mgr, const char *word)
{
	t_list				*lst;

	if (!word || !mgr)
		return (NULL);
	lst = NULL;
	dvprint(_MOD_ ": %s: Got _%s_\n", __FUNCTION__, word);
	if (NULL != ft_strchrs_betw(_skip_delims(word, IFS), _skip_delims_post(word,
				IFS), IFS))
	{
		dprint(_MOD_ ": %s: splitting\n", __FUNCTION__);
		lst = _do_split(mgr, word);
		ft_lstprint(lst);
	}
	return (lst);
}


/* -------------------------------------------------------------------------
 * Function Group: Token splitting handler logic
 * Description   : Handles splitting and re-inserting into t_list 
 * ------------------------------------------------------------------------- */

static void	_insert_token(t_mem_mgr *mgr, t_list **const lst_pos,
	t_list **const ins_lst, void *(*createf)(t_mem_mgr *, const void *))
{
	t_tok	*new_tok;

	new_tok = NULL;
	dvprint(_MOD_ ": %s: Inserting single tok _%s_\n", __FUNCTION__,
		(*ins_lst)->content);
	new_tok = createf(mgr, (*ins_lst)->content);
	if (!new_tok)
		exit_clean(&mgr->list, ENOMEM, __FUNCTION__, EMSG_MALLOC);
	tok_print(new_tok);
	new_tok->t.tok.is_combinable = true;
	destroy_token(mgr, &(*lst_pos)->content);
	(*lst_pos)->content = new_tok;
}

static void	_insert_toklst(t_mem_mgr *mgr, t_list **const lst_pos,
		t_list **const ins_lst, void *(*createf)(t_mem_mgr *, const void *))
{
	t_list	*new_lst;
	bool	reattach_head;
	t_list	*head;

	reattach_head = false;
	new_lst = NULL;
	dvprint(_MOD_ ": %s: Inserting lst %p\n", __FUNCTION__, *ins_lst);
	new_lst = ft_lstmap_tmp(mgr, *ins_lst, createf, destroy_token);
	((t_tok *)((ft_lstfirst(new_lst))->content))->t.tok.is_combinable = true;
	ft_lstadd_insert(lst_pos, new_lst);
	head = ft_lstfirst(*lst_pos);
	if (head == *lst_pos)
		reattach_head = true;
	ft_lstdelone_rwd_tmp(mgr, &head, lst_pos, destroy_token);
	if (reattach_head)
		*lst_pos = head;
}

/*
 * ins_lst is incoming list of split strings to be inserted
 * at the lst_pos position in a grparg token list, overwriting it.
 * Deletes lst_pos, deep copies insert list and inserts
 * at lst_pos position.
 * ins_lst is then destroyed.
 * Sets is_combinable on the first token. (Use with create_tmp_token)
 */
int	do_tok_inserts(t_mem_mgr *mgr, t_list **lst_pos, t_list **ins_lst,
		void *(*createf)(t_mem_mgr *, const void *))
{
	if (ft_lstsize(*ins_lst) > 1)
	{
		_insert_toklst(mgr, (t_list **const)lst_pos,
			(t_list **const)ins_lst, createf);
	}
	else
	{
		_insert_token(mgr, (t_list **const)lst_pos,
			(t_list **const)ins_lst, createf);
	}
	ft_lstclear_str_tmp(mgr, ins_lst);
	return (0);
}

#define DBGMSG_MATCHES "%s: %s split %d words, 1st: %s\n"

// Takes list of tokens (lst_pos) and inserts list of split tokens
// at lst_pos node
int	tok_do_wordsplits(t_mem_mgr *mgr, t_list **lst_pos, void *lst_c)
{
	t_list		*str_lst;
	const t_tok	*tok = (t_tok *)lst_c;

	str_lst = NULL;
	dvprint(_MOD_ ": %s: Got _%s_\n", __FUNCTION__, tok->t.tok.raw);
	if (false == tok->t.tok.in_dquotes)
	{
		str_lst = split_word(mgr, tok->t.tok.raw);
		if (str_lst)
		{
			dvprint(DBGMSG_MATCHES, _MOD_, __FUNCTION__, ft_lstsize(str_lst),
				str_lst->content);
			do_tok_inserts(mgr, lst_pos, &str_lst, create_split_token);
			ft_lstclear_str_tmp(mgr, &str_lst);
			dvprint(_MOD_ ": %s: Done.\n", __FUNCTION__);
		}
		else
			dvprint(_MOD_ ": %s: Nothing split or inserted.\n", __FUNCTION__);
	}
	return (0);
}
