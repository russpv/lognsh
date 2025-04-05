/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   token_utils_combine_utils_2.c                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dayeo <dayeo@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/05 19:20:11 by dayeo             #+#    #+#             */
/*   Updated: 2025/04/05 19:23:20 by dayeo            ###   ########.fr       */
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

// Whether contents was a quoted expansion, trims, then commits list
int	_put_str_on_toklst(t_state *s, char **str)
{
	char		*tmp;
	t_list		**tok_lst;

	if (!s || !str)
		return (ERR_ARGS);
	tok_lst = get_tmp_tok_list(s);
	if (UNQUOTED == get_tmp_flag(s))
		*str = _do_trim(s, *str, IFS, BACK);
	tmp = ft_strdup_tmp(get_mem(s), *str);
	if (!tmp)
		exit_clean(&get_mem(s)->list, ENOMEM, __FUNCTION__, EMSG_MALLOC);
	ft_lstadd_back(tok_lst, ft_lstnew_tmp(get_mem(s), \
		create_tmp_token(get_mem(s), tmp)));
	dealloc_str(s, *str);
	set_tmp_flag(s, INITVAL);
	*str = NULL;
	return (0);
}

// If something in cache, join it with raw regardless of delims
// Else, place in tmp_str
int	_load_tmp_normal(t_state *s, char *raw, char **str)
{
	if (true == _is_joinable(s, *str))
		*str = _do_join(s, *str, raw);
	else
	{
		if (*str)
			_put_str_on_toklst(s, str);
		*str = raw;
	}
	set_tmp_flag(s, LTRL);
	return (0);
}

// Ignores delimiters for quoted expansion tokens
int	_load_str_expanded_quoted(t_state *s, char *raw, char **str)
{
	if (true == _is_joinable(s, *str))
		*str = _do_join(s, *str, raw);
	else
	{
		if (*str)
			_put_str_on_toklst(s, str);
		*str = raw;
	}
	set_tmp_flag(s, QUOTED);
	return (0);
}

// Minds delimiters for unquoted expansion tokens
int	_load_str_expanded(t_state *s, char *raw, char **str)
{
	if (*str)
	{
		if (false == has_leading_delims(raw, IFS) && \
			false == has_lagging_delims(*str, IFS))
		{
			*str = _do_join(s, *str, raw);
			set_tmp_flag(s, UNQUOTED);
		}
		else if (true == has_leading_delims(raw, IFS))
		{
			_put_str_on_toklst(s, str);
			*str = _do_trim(s, raw, IFS, FRONT);
			set_tmp_flag(s, UNQUOTED);
		}
	}
	else
	{
		*str = _do_trim(s, raw, IFS, FRONT);
		set_tmp_flag(s, UNQUOTED);
	}
	debug_print(_MOD_ ": %s: loaded _%s_\n", *str);
	return (0);
}
