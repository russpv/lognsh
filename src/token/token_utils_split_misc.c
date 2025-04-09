#include "token_int.h"

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
	dprint(_MOD_ ": %s: Got _%s_\n", __FUNCTION__, word);
	if (NULL != ft_strchrs_betw(_skip_delims(word, IFS), _skip_delims_post(word,
				IFS), IFS))
	{
		dprint(_MOD_ ": %s: splitting\n", __FUNCTION__);
		lst = _do_split(mgr, word);
		ft_lstprint(lst);
	}
	return (lst);
}
