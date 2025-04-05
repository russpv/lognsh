/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   token_utils_combine.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dayeo <dayeo@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/05 09:10:30 by dayeo             #+#    #+#             */
/*   Updated: 2025/04/05 19:23:34 by dayeo            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "token_int.h"

#define SAYGOT 0
#define SAYCANCOMB 1
#define SAYDONE 2
#define SAYNULL 3
#define SAYCOMBINE 4
#define SAYTRIMMED 5
#define SAYCACHEONLY 6
#define SAYCANNOTCOMB 7

#define COMBINED 0
#define NOTCOMBINED 1

#define UNQUOTED 1
#define QUOTED 0
#define LTRL 2
#define INITVAL -1

#define FRONT 0
#define BACK 1

bool	_is_joinable(t_state *s, char *str)
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

// If no expansion, joins tokens.
// If expansion, processing and rules required.
// -- any delimiters on the boundary prevent joining, 
//	but must be trimmed before committing
// -- only the first token of a split expansion are 
//	left-joinable (last token can accept a join)
// -- if it was quoted, treat like no-expansion 
// token (don't mess with trimming/delimiters)
// Note: only the non-initial unquoted expansion tokens can be uncombinable
static void	_do_combine(t_state *s, const t_tok *content, char **str)
{
	if (true == content->t.tok.is_combinable)
	{
		if (true == content->t.tok.do_expansion \
			&& false == content->t.tok.in_dquotes)
			_load_str_expanded(s, content->t.tok.raw, str);
		else if (true == content->t.tok.do_expansion && \
			true == content->t.tok.in_dquotes)
			_load_str_expanded_quoted(s, content->t.tok.raw, str);
		else
			_load_tmp_normal(s, content->t.tok.raw, str);
	}
	else
	{
		if (*str)
			_put_str_on_toklst(s, str);
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
		return (0);
	}
	if (NULL == content->t.tok.raw)
		return (0);
	if (0 != *(content->t.tok.raw))
		_do_combine(s, content, &str);
	if (str)
		set_tmp(s, str);
	return (0);
}
