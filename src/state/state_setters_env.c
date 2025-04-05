/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   state_setters_env.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dayeo <dayeo@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/05 11:56:57 by dayeo             #+#    #+#             */
/*   Updated: 2025/04/05 11:56:59 by dayeo            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "state_int.h"

int	set_oldpwd(t_state *s, const char *caller)
{
	char	*old_pwd;

	old_pwd = get_pwd(s);
	if (!old_pwd || 0 != env_upsert_value(get_mem(s), get_env_list(s),
			OLDPWD_KEY, old_pwd))
	{
		print_custom_err(caller, "Corrupted working directory.\n");
		return (ERR_GENERAL);
	}
	return (0);
}

void	set_pwd(t_state *s)
{
	t_env	*lst;
	char	*pwd;
	char	*tmp;
	char	*tmp2;

	if (!s)
		return ;
	lst = get_env_list(s);
	if (!lst)
		return ;
	pwd = getcwd(NULL, 0);
	if (!pwd)
		return (perror("Cannot retrieve working directory\n"));
	tmp = ft_strdup_tmp(get_mem(s), pwd);
	if (!tmp)
		exit_clean(&get_mem(s)->list, ENOMEM, __FUNCTION__, EMSG_MALLOC);
	free(pwd);
	env_upsert_value(get_mem(s), lst, CWD_KEY, tmp);
	tmp2 = ft_strdup_tmp(get_mem(s), tmp);
	s->pwd = tmp2;
	debug_print("Set pwd:%s\n", s->pwd);
}

void	init_env_vars(t_state *s)
{
	if (!s)
		return ;
	_set_prompt(s, DFL_PROMPT);
	_set_lvl(s);
	set_pwd(s);
}
