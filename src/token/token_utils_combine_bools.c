/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   token_utils_combine_bools.c                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rpeavey <rpeavey@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/10 16:35:15 by rpeavey           #+#    #+#             */
/*   Updated: 2025/04/10 16:35:16 by rpeavey          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "token_int.h"

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
