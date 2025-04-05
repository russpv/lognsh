/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   state_setters_env_utils.c                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dayeo <dayeo@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/05 20:41:43 by dayeo             #+#    #+#             */
/*   Updated: 2025/04/05 20:42:12 by dayeo            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "state_int.h"

void	_set_prompt(t_state *s, char *str)
{
	t_env	*lst;

	if (!s || !str)
		return ;
	lst = get_env_list(s);
	if (lst)
		env_upsert_value(get_mem(s), lst, PROMPT_KEY, str);
}

void	_set_lvl(t_state *s)
{
	t_env	*lst;
	char	*value;
	int		lvl;

	if (!s)
		return ;
	lst = get_env_list(s);
	if (!lst)
		return ;
	value = env_find_value(LVL_KEY, lst);
	if (!value)
		lvl = 1;
	else
		lvl = ft_atoi(value) + 1;
	value = ft_itoa_mem(&s->mem_mgr.list, s->mem_mgr.f, lvl);
	if (!value)
		exit_clean(&s->mem_mgr.list, ENOMEM, __FUNCTION__, EMSG_MALLOC);
	env_upsert_value(get_mem(s), lst, LVL_KEY, value);
}

// Updates internal PATH with string delimited by :
void	set_path(t_state *s, const char *value)
{
	struct s_mem_utils	mem;

	if (!s)
		return ;
	mem_struct_init(get_mem(s), &mem);
	debug_print(_MOD_ ": setting path\n");
	if (NULL != s->path)
		ft_freearr_mem(mem.head, mem.u, (void **)s->path, -1);
	debug_print("Got path:%s\n", value);
	s->path = ft_split_mem(&mem, value, ':');
}
