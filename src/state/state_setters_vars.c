/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   state_setters_vars.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rpeavey <rpeavey@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/10 16:35:00 by rpeavey           #+#    #+#             */
/*   Updated: 2025/04/10 16:35:01 by rpeavey          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "state_int.h"

static void	_set_prompt(t_state *s, char *str)
{
	t_env	*lst;

	if (!s || !str)
		return ;
	lst = get_env_list(s);
	if (lst)
		env_upsert_value(get_mem(s), lst, PROMPT_KEY, str);
}

static void	_set_lvl(t_state *s)
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

void	init_env_vars(t_state *s)
{
	if (!s)
		return ;
	_set_prompt(s, DFL_PROMPT);
	_set_lvl(s);
	set_pwd(s);
}
