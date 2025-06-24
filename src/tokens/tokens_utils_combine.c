#include "tokens_int.h"

/* -------------------------------------------------------------------------
 * Function Group: Token combination logic helpers
 * Description   : Handles arcane bash-style re-combination of expansions  
 *   and splits
 * ------------------------------------------------------------------------- */

bool	has_leading_delims(char const *s, char const *set)
{
	char	*tmp;

	if (!s || !set)
		return (ERR_ARGS);
	tmp = (char *)s;
	while (ft_is_set(tmp, set))
		tmp++;
	if (tmp != s)
		return (true);
	return (false);
}

bool	has_lagging_delims(char const *s, char const *set)
{
	char	*end;
	char	*tmp;

	if (!s || !set)
		return (ERR_ARGS);
	end = ft_strchr(s, 0);
	tmp = end;
	if (end != s)
	{
		while (ft_is_set(--tmp, set))
			;
		if (--end != tmp)
			return (true);
	}
	return (false);
}

bool	is_joinable(t_state *s, char *str)
{
	if (!s || !str)
		return (false);
	if (UNQUOTED == get_tmp_flag(s))
	{
		if (false == has_lagging_delims(str, IFS))
			return (true);
	}
	if (QUOTED == get_tmp_flag(s) || LTRL == get_tmp_flag(s))
		return (true);
	return (false);
}

// Whether contents was a quoted expansion, trims, then commits list
int	put_str_on_toklst(t_state *s, char **str)
{
	char	*tmp;
	t_list	**tok_lst;

	if (!s || !str)
		return (ERR_ARGS);
	tok_lst = get_tmp_tok_list(s);
	dprint(_MOD_ ": %s: _%s_\n", __FUNCTION__, *str);
	if (UNQUOTED == get_tmp_flag(s))
		*str = do_trim(s, *str, IFS, BACK);
	tmp = ft_strdup_mem(get_mem(s), *str);
	if (!tmp)
		exit_clean(&get_mem(s)->list, ENOMEM, __FUNCTION__, EMSG_MALLOC);
	ft_lstadd_back(tok_lst, ft_lstnew_tmp(get_mem(s),
			create_tmp_token(get_mem(s), tmp)));
	(get_mem(s))->dealloc(&get_mem(s)->list, *str);
	set_tmp_flag(s, INITVAL);
	*str = NULL;
	return (0);
}

char	*do_join(t_state *s, char *left, char *right)
{
	char				*res;
	struct s_mem_utils	m;

	if (!s || !left || !right)
		return (NULL);
	mem_struct_init(get_mem(s), &m);
	res = ft_strjoin_mem(m.head, m.f, left, right);
	if (!res)
		exit_clean(m.head, ENOMEM, __FUNCTION__, EMSG_MALLOC);
	return (res);
}

char	*do_trim(t_state *s, char *str, char *set, int side)
{
	char				*res;
	struct s_mem_utils	m;

	if (!s || !s || !set)
		return (NULL);
	mem_struct_init(get_mem(s), &m);
	if (FRONT == side)
		res = ft_strtrimfront_mem(&m, str, set);
	else
		res = ft_strtrimback_mem(&m, str, set);
	if (!res)
		exit_clean(m.head, ENOMEM, __FUNCTION__, EMSG_MALLOC);
	return (res);
}

void	announce(const char *cllr, char *raw, char *str, int msg)
{
	if (!cllr)
		return ;
	if (SAYGOT == msg)
	{
		if (str)
			dvprint(_MOD_ ": %s: got _%s_, _%s_(cache)\n", cllr, raw, str);
		else
			dvprint(_MOD_ ": %s: got _%s_\n", cllr, raw);
	}
	else if (SAYDONE == msg)
		dvprint(_MOD_ ": %s: returning _%s_(cache)\n", cllr, str);
	else if (SAYCANCOMB == msg)
		dvprint(_MOD_ ": %s: got comb'ble _%s_\n", cllr, raw);
	else if (SAYCANNOTCOMB == msg)
		dvprint(_MOD_ ": %s: got uncomb'ble %s, add'g cached _%s_\n", cllr, raw,
			str);
	else if (SAYCOMBINE == msg)
		dvprint(_MOD_ ": %s: combn'g _%s_ + _%s_(cache)\n", cllr, raw, str);
	else if (SAYTRIMMED == msg)
		dvprint(_MOD_ ": %s: only trim frnt _%s_ + rr _%s_\n", cllr, raw, str);
	else if (SAYCACHEONLY == msg)
		dvprint(_MOD_ ": %s: only caching _%s_, can't comb _%s_(cache)\n", cllr,
			raw, str);
	else
		dvprint(_MOD_ ": %s: got NULL\n", cllr);
}


/* -------------------------------------------------------------------------
 * Function Group: Token combination logic
 * Description   : 
 * ------------------------------------------------------------------------- */

// If something in cache, join it with raw regardless of delims
// Else, place in tmp_str
static int	_load_tmp_normal(t_state *s, char *raw, char **str)
{
	if (true == is_joinable(s, *str))
	{
		announce(__FUNCTION__, raw, *str, SAYCOMBINE);
		*str = do_join(s, *str, raw);
	}
	else
	{
		announce(__FUNCTION__, raw, *str, SAYCANNOTCOMB);
		if (*str)
			put_str_on_toklst(s, str);
		*str = raw;
	}
	set_tmp_flag(s, LTRL);
	return (0);
}

// Ignores delimiters for quoted expansion tokens
static int	_load_str_expanded_quoted(t_state *s, char *raw, char **str)
{
	if (true == is_joinable(s, *str))
	{
		announce(__FUNCTION__, raw, *str, SAYCOMBINE);
		*str = do_join(s, *str, raw);
	}
	else
	{
		announce(__FUNCTION__, raw, *str, SAYCANNOTCOMB);
		if (*str)
			put_str_on_toklst(s, str);
		*str = raw;
	}
	set_tmp_flag(s, QUOTED);
	return (0);
}

// Minds delimiters for unquoted expansion tokens
static int	_load_str_expanded(t_state *s, char *raw, char **str)
{
	if (*str)
	{
		if (false == has_leading_delims(raw, IFS)
			&& false == has_lagging_delims(*str, IFS))
		{
			announce(__FUNCTION__, raw, *str, SAYCOMBINE);
			*str = do_join(s, *str, raw);
			set_tmp_flag(s, UNQUOTED);
		}
		else if (true == has_leading_delims(raw, IFS))
		{
			put_str_on_toklst(s, str);
			*str = do_trim(s, raw, IFS, FRONT);
			set_tmp_flag(s, UNQUOTED);
			announce(__FUNCTION__, NULL, *str, SAYTRIMMED);
		}
	}
	else
	{
		*str = do_trim(s, raw, IFS, FRONT);
		set_tmp_flag(s, UNQUOTED);
		announce(__FUNCTION__, raw, NULL, SAYCACHEONLY);
	}
	dprint(_MOD_ ": %s: loaded _%s_\n", *str);
	return (0);
}

// If no expansion, joins tokens.
// If expansion, processing and rules required.
// -- any delimiters on the boundary prevent joining,
//	but must be trimmed before committing
// -- only the first token of a split expansion are left-joinable 
//		(last token can accept a join)
// -- if it was quoted,
//	treat like no-expansion token (don't mess with trimming/delimiters)
// Note: only the non-initial unquoted expansion tokens can be uncombinable
static void	_do_combine(t_state *s, const t_tok *content, char **str)
{
	if (true == content->t.tok.is_combinable)
	{
		announce(__FUNCTION__, content->t.tok.raw, NULL, SAYCANCOMB);
		if (true == content->t.tok.do_expansion
			&& false == content->t.tok.in_dquotes)
			_load_str_expanded(s, content->t.tok.raw, str);
		else if (true == content->t.tok.do_expansion
			&& true == content->t.tok.in_dquotes)
			_load_str_expanded_quoted(s, content->t.tok.raw, str);
		else
			_load_tmp_normal(s, content->t.tok.raw, str);
	}
	else
	{
		announce(__FUNCTION__, content->t.tok.raw, *str, SAYCANNOTCOMB);
		if (*str)
			put_str_on_toklst(s, str);
		*str = content->t.tok.raw;
		set_tmp_flag(s, UNQUOTED);
	}
	return ;
}

/* Passed to token llist iterator to collect word parts with successive calls.
// Discards null raw strings.
// If c is a boundary token, doesn't join if it has delimiters
*/
int	tok_do_grp_combine(t_state *s, void *c)
{
	const t_tok	*content = (t_tok *)c;
	static char	*str = NULL;

	if (NULL == c)
	{
		str = NULL;
		return (announce(__FUNCTION__, NULL, NULL, SAYNULL), 0);
	}
	announce(__FUNCTION__, content->t.tok.raw, str, SAYGOT);
	if (NULL == content->t.tok.raw)
		return (0);
	if (0 != *(content->t.tok.raw))
		_do_combine(s, content, &str);
	if (str)
		set_tmp(s, str);
	announce(__FUNCTION__, NULL, str, SAYDONE);
	return (0);
}
