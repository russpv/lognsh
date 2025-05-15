/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   token_utils_combine_misc.c                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rpeavey <rpeavey@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/10 16:35:16 by rpeavey           #+#    #+#             */
/*   Updated: 2025/04/10 16:35:17 by rpeavey          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "token_int.h"

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
	tmp = ft_strdup_tmp(get_mem(s), *str);
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
