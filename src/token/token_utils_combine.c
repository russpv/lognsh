#include "token_int.h"


# define SAYGOT 0
# define SAYCANCOMB 1
# define SAYDONE 2
# define SAYNULL 3
# define SAYCOMBINE 4
# define SAYTRIMMED 5
# define SAYCACHEONLY 6
# define SAYCANNOTCOMB 7

# define COMBINED 0
# define NOTCOMBINED 1

static void _announce(const char *caller, char *raw, char *str, int msg)
{
	if (!caller)
		return ;
	if (SAYGOT == msg)
	{
		if (str)
			debug_print(_MOD_ ": %s: got _%s_, _%s_(cache)\n", caller, raw, str);
		else
			debug_print(_MOD_ ": %s: got _%s_\n", caller, raw);
	}
	else if (SAYDONE == msg)
		debug_print(_MOD_ ": %s: returning _%s_(cache)\n", caller, str);
	else if (SAYCANCOMB == msg)
		debug_print(_MOD_ ": %s: got combinable _%s_\n", caller, raw);
	else if (SAYCANNOTCOMB == msg)
		debug_print(_MOD_ ": %s: got uncombinable, commiting _%s_\n", caller, raw);
	else if (SAYCOMBINE == msg)
		debug_print(_MOD_ ": %s: combining _%s_ + _%s_(cache)\n", caller, raw, str);
	else if (SAYTRIMMED == msg)
		debug_print(_MOD_ ": %s: only trimmed front _%s_, and rear _%s_\n", caller, raw, str);
	else if (SAYCACHEONLY == msg)
		debug_print(_MOD_ ": %s: only caching _%s_, can't combine _%s_(cache)\n", caller, raw, str);
	else
		debug_print(_MOD_ ": %s: got NULL\n", caller);
}

bool has_leading_delims(char const *s, char const *set)
{
	char *tmp;

	tmp = (char*)s;
	while (ft_is_set(tmp, set))
		tmp++;
	if (tmp != s)
		return (true);
	return (false);
}

bool has_lagging_delims(char const *s, char const *set)
{
	char *end;
	char *tmp;

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

static int _load_str_normal(t_state *s, char *raw, char **str, char **tmp_str)
{
	struct s_mem_utils m;
	int combined;

	m.f = get_mem(s)->f;
	m.u = get_mem(s)->dealloc;
	m.head = &get_mem(s)->list;
	combined = NOTCOMBINED;
	if (*str)
	{
		_announce(__FUNCTION__, raw, *str, SAYCOMBINE);
		*tmp_str = ft_strjoin_mem(m.head, m.f, *str, raw);
		combined = COMBINED;
	}
	else
	{
		*tmp_str = raw;
		_announce(__FUNCTION__, raw, NULL, SAYCACHEONLY);
	}
	return (combined);
}

static int _load_str_post_expansion(t_state *s, char *raw, char **str, char **tmp_str)
{
	struct s_mem_utils m;
	int combined;

	m.f = get_mem(s)->f;
	m.u = get_mem(s)->dealloc;
	m.head = &get_mem(s)->list;
	combined = NOTCOMBINED;
	if (*str)
	{
		if (false == has_leading_delims(raw, IFS) && false == has_lagging_delims(*str, IFS))
		{
			_announce(__FUNCTION__, raw, *str, SAYCOMBINE);
			*tmp_str = ft_strjoin_mem(m.head, m.f, *str, raw); // join, decide tokenize later
			combined = COMBINED;
		}
		else if (true == has_lagging_delims(*str, IFS))
		{
			fprintf(stderr, "has lagging delims %s\n", *str);
			*tmp_str = *str;
			*str = ft_strtrimback_mem(&m, *tmp_str, IFS); // don't join, trim whats in cache, 
			*tmp_str = ft_strtrimfront_mem(&m, raw, IFS); // trim incoming and pass by reference
			_announce(__FUNCTION__, *tmp_str, *str, SAYTRIMMED);
		}
	}
	else //nothing to combine. cache token.
	{
		fprintf(stderr, "no combine, trimming token %s\n", raw);
		*tmp_str = ft_strtrimfront_mem(&m, raw, IFS); //trim incoming and pass by reference
		_announce(__FUNCTION__, raw, NULL, SAYCACHEONLY);
	}
	fprintf(stderr, "tmp_str %s\n", *tmp_str);	
	return (combined);
}

static int	_put_str_on_toklst(t_state *s, char *str)
{
	char *tmp;

	t_list **tok_lst;

	if (!s || !str)
		return (ERR_ARGS);
	tok_lst = get_tmp_tok_list(s);
	fprintf(stderr, "%s\n", __FUNCTION__);	
	tmp = ft_strdup_tmp(get_mem(s), str);
	if (!tmp)
		exit_clean(&get_mem(s)->list, ENOMEM, __FUNCTION__, EMSG_MALLOC);
	ft_lstadd_back(tok_lst, ft_lstnew_tmp(get_mem(s), create_tmp_token(get_mem(s), tmp)));
	tok_print_list(*tok_lst);
	return (0);
}

// Either loads the static str cache for combinable raws, or commits tokens and clears cache
static void	_do_combine(t_state *s, const t_tok	*content, char	**str, int *combined)
{
	char *tmp_str;

	tmp_str = NULL;
	if (true == content->t.tok.is_combinable) //either combined with previous, or modified cache and passed tmp_str
	{
		_announce(__FUNCTION__, content->t.tok.raw, NULL, SAYCANCOMB);
		if (true == content->t.tok.do_expansion && false == content->t.tok.in_dquotes)
			*combined = _load_str_post_expansion(s, content->t.tok.raw, str, &tmp_str);
		else
			*combined = _load_str_normal(s, content->t.tok.raw, str, &tmp_str);
		fprintf(stderr, "commiting str %s got tmp_str %s\n", *str, tmp_str );
		if (*str)
			_put_str_on_toklst(s, *str);
		*str = NULL;
		if (tmp_str)
			*str = tmp_str;
	}
	else
	{
		_announce(__FUNCTION__, content->t.tok.raw, NULL, SAYCANNOTCOMB);
		if (*str)
			_put_str_on_toklst(s, *str);
		*str = NULL;
		_put_str_on_toklst(s, content->t.tok.raw);
	}
}

// Passed to token llist iterator to collect word parts
// on static str with successive calls.
// Discards null raw strings.
// c must be a t_tok
// If c is a boundary token, checks if either end has space
// if space, trims both ends, does not combine
// if is result of globbing, does not combine
int	tok_do_grp_combine(t_state *s, void *c)
{
	const t_tok	*content = (t_tok *)c;
	static char	*str = NULL;
	t_list		**tok_lst;
	int			combined;

	combined = NOTCOMBINED;
	tok_lst = get_tmp_tok_list(s);
	if (NULL == c)
	{
		str = NULL;
		return (_announce(__FUNCTION__, NULL, NULL, SAYNULL), 0);
	}
	_announce(__FUNCTION__, content->t.tok.raw, str, SAYGOT);
	if (NULL == content->t.tok.raw) // Nothing to process
		return (0);
	if (0 != *(content->t.tok.raw))
		_do_combine(s, content, &str, &combined);
	if (str && combined == COMBINED)
	{ 	// commit if we have cached aggregation and need to start new cache
		fprintf(stderr, RED"commiting (delete this branch?)%s \n"RESET, str );
		_put_str_on_toklst(s, str);
		str = NULL;
		set_tmp(s, str);
	}
	//if (str) //save to cache
	//	set_tmp(s, str);
	_announce(__FUNCTION__, NULL, str, SAYDONE);
	return (0);
}
