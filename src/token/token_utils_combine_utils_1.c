/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   token_utils_combine_utils_1.c                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dayeo <dayeo@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/05 19:19:22 by dayeo             #+#    #+#             */
/*   Updated: 2025/04/05 19:23:26 by dayeo            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

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

#include "token_int.h"

char	*_do_join(t_state *s, char *left, char *right)
{
	struct s_mem_utils	m;
	char				*res;

	if (!s || !left || !right)
		return (NULL);
	mem_struct_init(get_mem(s), &m);
	res = ft_strjoin_mem(m.head, m.f, left, right);
	if (!res)
		exit_clean(m.head, ENOMEM, __FUNCTION__, EMSG_MALLOC);
	return (res);
}

char	*_do_trim(t_state *s, char *str, char *set, int side)
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
