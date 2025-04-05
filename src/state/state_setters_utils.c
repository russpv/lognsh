/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   state_setters_utils.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dayeo <dayeo@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/05 20:37:44 by dayeo             #+#    #+#             */
/*   Updated: 2025/04/05 20:38:27 by dayeo            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "state_int.h"

/* 
void	set_tmp(t_state *s, char *str)
{
	if (NULL != s->tmp)
		get_mem(s)->dealloc(&get_mem(s)->list, s->tmp);
	s->tmp = ft_strdup_tmp(get_mem(s), str);
}*/

//Refactored set_tmp which complies with norm
void	set_tmp(t_state *s, char *str)
{
	t_mem_mgr	*mem_mgr;

	if (NULL != s->tmp)
	{
		mem_mgr = get_mem(s);
		mem_mgr->dealloc(&mem_mgr->list, s->tmp);
	}
	s->tmp = ft_strdup_tmp(get_mem(s), str);
}

void	set_got_heredoc(t_state *s)
{
	if (NULL == s)
		return ;
	s->got_heredoc = true;
}

void	set_tmp_flag(t_state *s, int val)
{
	if (NULL == s)
		return ;
	s->tmp_flag = val;
}
