#include "token_int.h"

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
