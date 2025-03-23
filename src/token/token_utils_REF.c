#include "token_int.h"

#define DBGMSG_MATCHES _MOD_ ": %s split %d words, 1st: %s\n"

int	tok_print(void *content)
{
	t_tok	*tok;

	tok = (t_tok *)content;
	if (tok)
	{
		if (GROUP == tok->class)
		{
			log_print(_MOD_ ": Grp_tok: (grp) Exp: - Glb: - Sub: - tokc: %d \n",
				tok->t.meta.tokc);
			if (0 != ft_lstiter(tok->t.meta.tokens, tok_print))
				err("LSTITER");
		}
		else if (tok_get_issubtoken(tok))
			log_print(_MOD_ ": Sub_tok: %02d Exp: %d Glb: %d Sub: %d Dq: %d Val: \
				%s \n", tok->t.tok.type, tok->t.tok.do_expansion,
				tok->t.tok.do_globbing, tok->t.tok.is_subtoken, tok->t.tok.in_dquotes, tok->t.tok.raw);
		else
			log_print(_MOD_ ": Token: %02d Exp: %d Glb: %d Sub: %d Dq: %d Val: %s \n",
				tok->t.tok.type, tok->t.tok.do_expansion,
				tok->t.tok.do_globbing, tok->t.tok.is_subtoken, tok->t.tok.in_dquotes, tok->t.tok.raw);
	}
	return (0);
}

void	tok_print_list(t_list *head)
{
	t_list	*current;
	int		i;

	current = head;
	i = 0;
	if (DEBUG && isatty(STDOUT_FILENO))
	{
		while (current)
		{
			printf("Node %d:\n", i);
			tok_print(current->content);
			current = current->next;
			i++;
		}
	}
	printf("List printed.\n");
}


// Passed to llist iterator to collect word parts
// c must be a t_tok
// Stores result in s->tmp, which is later
// assigned to grp token's raw string
// Memory allocations are local to this scope.
int	tok_do_grp_combine(t_state *s, void *c)
{
	const t_tok	*content = (t_tok *)c;
	static char	*str = NULL;
	char		*tmp;

	tmp = NULL;
	if (NULL == c)
	{
		debug_print(_MOD_ ": %s: got NULL\n", __FUNCTION__);
		free(str);
		str = NULL;
		return (0);
	}
	debug_print(_MOD_ ": %s: got _%s_\n", __FUNCTION__, content->t.tok.raw);
	if (NULL == content->t.tok.raw)
		return (0);
	if (0 != *(content->t.tok.raw))
	{
		if (str)
			tmp = ft_strjoin(str, content->t.tok.raw);
		else
			tmp = ft_strdup(content->t.tok.raw);
		if (!tmp)
			exit_clean(&get_mem(s)->list, ENOMEM, __FUNCTION__, EMSG_MALLOC);
	}
	if (tmp)
	{
		if (NULL != str)
			free(str);
		str = tmp;
		set_tmp(s, str);
		debug_print(_MOD_ ": %s: returning _%s_\n", __FUNCTION__, get_tmp(s));
	}
	return (0);
}

/* Looks for env values of key loaded in buf, replaces raw. */
static int	_do_tok_ops(t_state *s, const t_tok *c, char *buf, char **value)
{
	char	*new_raw;

	new_raw = NULL;
	if (check_special_expansions(s, buf, value) < 0)
	{
		if (*value)
		{
			myfree(&get_mem(s)->list, c->t.tok.raw);
			((t_tok *)c)->t.tok.raw = *value;
		}
	}
	else
	{
		debug_print(_MOD_ ": %s: checking envp\n", __FUNCTION__);
		*value = get_env_val(s, buf);
		if (*value)
		{
			new_raw = ft_strdup_tmp(get_mem(s), *value);
			if (!new_raw)
				exit_clean(&get_mem(s)->list, ENOMEM, __FUNCTION__,
					EMSG_MALLOC);
		}
		myfree(&get_mem(s)->list, c->t.tok.raw);
		((t_tok *)c)->t.tok.raw = new_raw;
	}
	return (0);
}

int	tok_do_expansion(t_state *s, void *c)
{
	char		*value;
	const t_tok	*content = (t_tok *)c;
	int			res;
	char		buf[MAX_ENVVAR_LEN];
	size_t		raw_len;

	res = 0;
	value = NULL;
	if (NULL == content)
		return (0);
	if (GROUP == content->class)
		return (0);
	raw_len = ft_strnlen(content->t.tok.raw, MAX_ENVVAR_LEN);
	ft_memset(buf, 0, sizeof(buf));
	debug_print(_MOD_ ": %s:  %s\n", __FUNCTION__, content->t.tok.raw);
	if (content->t.tok.do_expansion)
	{
		if (raw_len <= lex_get_keepd(get_lexer(s)))
			return (err("error"), ERR_ARGS);
		ft_memcpy(buf, content->t.tok.raw + lex_get_keepd(get_lexer(s)), raw_len
			- lex_get_keepd(get_lexer(s)));
		res = _do_tok_ops(s, content, buf, &value);
		if (0 != res)
			return (res);
		debug_print(_MOD_ ": %s: expanded var: %s\n", __FUNCTION__, value);
	}
	return (0);
}

// Returns list of strings
t_list	*split_word(t_mem_mgr *mgr, const char *word)
{
	t_list				*lst;
	t_list				*new;
	char				**split_raw;
	struct s_mem_utils	mem;
	int 				i;

	if (!word || !mgr)
		return (NULL);
	mem_struct_init(mgr, &mem);
	lst = NULL;
	i = -1;
	debug_print(_MOD_ ": %s: Got _%s_\n", __FUNCTION__, word);
	if (NULL != ft_strchrs(word, IFS))
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

// Explicit empty strings retained as empty strings. 
// Unquoted nulls resulting from expansions are removed.
// Quoted nulls resulting from expansions are retained as empty strings, (done prior)
// unless part of a non-null expansion word. -d'' is -d
// Returns list of strings
int	tok_do_wordsplits(t_mem_mgr *mgr, t_list **lst_pos, void *lst_c)
{
	t_list				*lst;
	const t_tok	*tok = (t_tok *)lst_c;

	lst = NULL;
	assert(NORMAL == tok->class);
	debug_print(_MOD_ ": %s: Got _%s_\n", __FUNCTION__, tok->t.tok.raw);
	tok_print_list(*lst_pos);
	if (false == tok->t.tok.in_dquotes)
	{
		lst = split_word(mgr, tok->t.tok.raw);
		if (lst)
		{
			debug_print(DBGMSG_MATCHES, __FUNCTION__, ft_lstsize(lst), lst->content);
			do_tok_inserts(mgr, lst_pos, &lst);
			tok_print_list(*lst_pos);
			debug_print(_MOD_ ": %s: Done.\n", __FUNCTION__);
		}
		else
			debug_print(_MOD_ ": %s: Nothing split or inserted.\n", __FUNCTION__);
	}
	return (0);
}

/*
 * ins_lst is incoming list of split strings to be inserted
 * at the lst_pos position in a grparg token list, overwriting it.
 * Deletes lst_pos, deep copies insert list and inserts
 * at lst_pos position.
 * ins_lst is then destroyed.
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
		new_lst = ft_lstmap_tmp(mgr, *ins_lst, create_tmp_token,\
			destroy_token);
		ft_lstadd_insert(lst_pos, new_lst);
		head = ft_lstfirst(*lst_pos);
		ft_lstdelone_rwd_tmp(mgr, &head, lst_pos, destroy_token);
	}
	else
	{
		debug_print(_MOD_ ": %s: Inserting single tok _%s_\n", __FUNCTION__, (*ins_lst)->content);
		new_tok = create_token(mgr, (*ins_lst)->content, NORMAL, -1);
		if (!new_tok)
			exit_clean(&mgr->list, ENOMEM, __FUNCTION__, EMSG_MALLOC);
		destroy_token(mgr, (*lst_pos)->content);
		(*lst_pos)->content = ft_lstnew_tmp(mgr, new_tok);
	}
	ft_lstclear_str_tmp(mgr, ins_lst);
	return (0);
}
